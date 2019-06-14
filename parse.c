#include "xcc.h"

static Vector *tokens;
static int pos;
static Vector *code;
// static int var_count;

static Map *vars;
static int offset;

int consume(int ty) {
    Token *t = tokens->data[pos];
    if (t->ty != ty)
        return 0;
    pos++;
    return 1;
}

static void expect(int ty) {
    Token *t = tokens->data[pos];
    if (!consume(ty))
        error_at(t->input, format("Not '%c'", ty));
}

static is_typename() {
    Token *t = tokens->data[pos];
    return t->ty == TK_INT;
}

Node *new_node(int ty, Node *lhs, Node *rhs) {
    Node *node = malloc(sizeof(Node));
    node->ty = ty;
    node->lhs = lhs;
    node->rhs = rhs;
    return node;
}

Node *new_node_num(int val) {
    Node *node = malloc(sizeof(Node));
    node->ty = ND_NUM;
    node->val = val;
    return node;
}

Node *new_node_ident(char *name) {
    Node *node = malloc(sizeof(Node));
    node->ty = ND_IDENT;
    node->name = name;
    return node;
}


Node *assign() {
    Node *node = equality();
    if (consume('='))
        node = new_node('=', node, assign());
    return node;
}

Node *expr() {
    return assign();
}

Node *stmt() {
    Node *node;

    if (consume(TK_RETURN)) {
        node = malloc(sizeof(Node));
        node->ty = ND_RETURN;
        node->lhs = expr();
        expect(';');
        return node;
    }
    else if (consume(TK_IF)) {
        node = malloc(sizeof(Node));
        node->ty = ND_IF;

        expect('(');
        node->cond = expr();
        expect(')');
        node->then = stmt();

        if (consume(TK_ELSE)) 
            node->els = stmt();
        
        return node;
    }
    else if (consume(TK_WHILE)) {
        node = malloc(sizeof(Node));
        node->ty = ND_WHILE;

        expect('(');
        node->cond = expr();
        expect(')');

        node->then = stmt();
        return node;
    }
    else if (consume(TK_FOR)) {
        node = malloc(sizeof(Node));
        node->ty = ND_FOR;

        expect('(');
        if (!consume(';')) {
            if (consume(TK_INT))
                node->init = declaration();
            else {
                node->init = expr();
                expect(';');
            }
        }

        if (!consume(';')) {
            node->cond = expr();
            expect(';');
        }

        if (!consume(')')) {
            node->inc = expr();
            expect(')');
        }

        node->body = stmt();
        return node;
    }
    else if (consume('{')) {
        node = malloc(sizeof(Node));
        node->ty = ND_BLOCK;
        node->stmts = new_vector();

        while (!consume('}')) {
            vec_push(node->stmts, stmt());
        }

        return node;
    }
    else if (consume(TK_INT)) {
        /// variable declaration
        node = declaration();
        return node;
    }
    else {
        /// default statement
        node = expr();
        expect(';');
        return node;
    }
}


Node *equality() {
    Node *node = relational();
    
    for (;;) {
        if (consume(TK_EQ))
            node = new_node(ND_EQ, node, relational());
        else if (consume(TK_NE))
            node = new_node(ND_NE, node, relational());
        else
            return node;        
    }
}

Node *relational() {
    Node *node = add();
    
    for (;;) {
        if (consume(TK_LE))
            node = new_node(ND_LE, node, add());
        else if (consume(TK_GE))
            node = new_node(ND_LE, add(), node);
        else if (consume('<'))
            node = new_node('<', node, add());
        else if (consume('>'))
            node = new_node('<', add(), node);
        else
            return node;        
    }
}

Node *add() {
    Node *node = mul();

    for (;;) {
        if (consume('+')) {
            node = new_node('+', node, mul());
            // scaling to adjust type length
            // TODO: can process lhs and rhs pointer
            if (node->lhs->ty == ND_IDENT) {
                Var *v = map_get(vars, node->lhs->name);
                if (v->ty->ty == PTR) {
                    node->rhs->val = node->rhs->val * 4;
                }
            }
        }

        else if (consume('-'))
            node = new_node('-', node, mul());
        else
            return node;
    }
}

Node *mul() {
    Node *node = unary();

    for (;;) {
        if (consume('*'))
            node = new_node('*', node, unary());
        else if (consume('/'))
            node = new_node('/', node, unary());
        else
            return node;
    }
}

Node *unary() {
    if (consume('*')) {
        Node *node = malloc(sizeof(Node));
        node->ty = ND_DEREF;
        node->expr = unary();
        return node;
    }
    if (consume('&')) {
        Node *node = malloc(sizeof(Node));
        node->ty = ND_ADDR;
        node->expr = unary();
        return node;
    }
    if (consume('+'))
        return term();
    if (consume('-'))
        return new_node('-', new_node_num(0), term());
    return term();
}

Node *term() {
    Token *t = tokens->data[pos];
    if (consume('(')) {
        Node *node = expr();
        expect(')');
        return node;
    }

    

    if (t->ty == TK_NUM) {
        pos++;
        return new_node_num(t->val);
    }

    if (t->ty == TK_IDENT) {
        pos++;
        Node *node;

        // Identifier
        if (!consume('(')) {
            if (map_get(vars, t->name) == NULL)
                error_at(t->input, "undefined variable");
            node = new_node_ident(t->name);
            return node;
        }

        // Function call
        node = malloc(sizeof(Node));
        node->ty = ND_CALL;
        node->name = t->name;
        node->args = new_vector();

        if (consume(')'))
            return node;

        vec_push(node->args, expr());
        while (consume(',')) {
            vec_push(node->args, expr());
        }
        expect(')');

        return node;
    }

    error_at(t->input, "non-number or opening parentheses Token found");

    return NULL;
}

Node *declaration() {

    int ptr_count = 0;
    while (consume('*'))
    {
        ptr_count++;
    }

    Token *t = tokens->data[pos];
    if (!consume(TK_IDENT))
        error_at(t->input, "not variable declaration");

    Node *node = malloc(sizeof(Node));
    node->ty = ND_VARDEF;
    node->name = t->name;

    if (map_get(vars, node->name) != NULL)
        error("'%s' is already defined", node->name);

    // variable setting
    Var *var = malloc(sizeof(Var));
    var->offset = offset;
    offset += 8;

    var->ty = malloc(sizeof(Type));
    var->ty->ty = INT; // Base type

    while (ptr_count > 0)
    {
        Type *prev = var->ty;
        var->ty = malloc(sizeof(Type));
        var->ty->ty = PTR;
        var->ty->ptr_to = prev;
        ptr_count--;
    }

    // check code for pointer
    // Type *cur = var->ty;
    // while (cur)
    // {
    //     printf("%d\n", cur->ty);
    //     cur = cur->ptr_to;
    // }

    map_put(vars, node->name, var);

    if (consume('=')) {
        node->init = assign();
    }

    expect(';');

    return node;
}

Node *param()
{
    expect(TK_INT);

    int ptr_count = 0;
    while (consume('*'))
    {
        ptr_count++;
    }

    Token *t = tokens->data[pos];
    if (!consume(TK_IDENT))
        error_at(t->input, "not variable declaration");

    Node *node = malloc(sizeof(Node));
    node->ty = ND_VARDEF;
    node->name = t->name;

    // variable setting
    Var *var = malloc(sizeof(Var));
    var->offset = offset;
    offset += 8;

    var->ty = malloc(sizeof(Type));
    var->ty->ty = INT; // Base type

    while (ptr_count > 0)
    {
        Type *prev = var->ty;
        var->ty = malloc(sizeof(Type));
        var->ty->ty = PTR;
        var->ty->ptr_to = prev;
        ptr_count--;
    }

    map_put(vars, node->name, var);

    return node;
}

Function *function() {
    if (!consume(TK_INT))
        error_at(((Token *)tokens->data[pos])->input, "Not type declaration");

    Token *t = tokens->data[pos];
    if (!consume(TK_IDENT))
        error_at(t->input, "Not function");

    Node *node = malloc(sizeof(Node));
    node->ty = ND_FUNC;
    node->name = t->name;
    node->args = new_vector();

    Function *fn = malloc(sizeof(Function));
    fn->node = node;

    // make space for local variables every time in function definition
    vars = new_map();
    offset = 8;
    fn->vars = vars;

    expect('(');

    if (!consume(')'))
    {
        vec_push(node->args, param());
        while (consume(','))
        {
            vec_push(node->args, param());
        }
        expect(')');
    }

    node->body = stmt();
    return fn;
}

void program() {
    while (((Token *)tokens->data[pos])->ty != TK_EOF)
        vec_push(code, function());
    vec_push(code, NULL);
}

Vector *parse(Vector *v) {
    tokens = v;
    pos = 0;
    code = new_vector();

    program();

    return code;
}