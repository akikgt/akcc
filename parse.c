#include "xcc.h"

static Vector *tokens;
static int pos;
static Program *prog;

static Map *vars;

static int offset;
static int str_count;


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

static int peek(int ty) {
    Token *t = tokens->data[pos];
    return t->ty == ty;
}

static int is_typename() {
    Token *t = tokens->data[pos];
    return t->ty == TK_INT || t->ty == TK_CHAR;
}

int sizeof_types(int ty) {
    switch (ty) {
        case TK_CHAR: return 1;
        case TK_INT: return 4;
        default:
            error("unknown type");
            break;
    }
    return 8;
}

static Type *type_specifier() {
    Type *ty = malloc(sizeof(Type));

    Token *t = tokens->data[pos++];
    switch (t->ty) {
        case TK_CHAR:
            ty->ty = CHAR;
            ty->size = 1;
            break;
        case TK_INT:
            ty->ty = INT;
            ty->size = 4;
            break;
    }

    while (consume('*')) {
        ty = ptr_to(ty);
    }

    return ty;
}

Type *ptr_to(Type *base) {
    Type *new_ty = malloc(sizeof(Type));
    new_ty->ty = PTR;
    new_ty->size = 8;
    new_ty->ptr_to = base;
    return new_ty;
}

static Type *arr_of(Type *base) {
    Type *ret = base;
    Vector *stack = new_vector();

    while (consume('[')) {
        Token *t = tokens->data[pos];
        if (!(t->ty == TK_NUM)) {
            vec_push(stack, (void *)0);
        }
        else {
            vec_push(stack, (void *)t->val);
            pos++;
        }
        expect(']');
    }

    for (int i = stack->len - 1; i >= 0; i--) {
        Type *new_ty = malloc(sizeof(Type));
        new_ty->ty = ARRAY;        
        new_ty->array_size = stack->data[i];
        new_ty->size = new_ty->array_size * ret->size;
        new_ty->arr_of = ret;
        new_ty->ptr_to = ret;
        ret = new_ty;
    }

    return ret;
}

Node *new_node(int op) {
    Node *node = malloc(sizeof(Node));
    node->op = op;
    return node;
}

Node *new_node_expr(int op, Node *expr) {
    Node *node = new_node(op);
    node->expr = expr;
    return node;
}

Node *new_node_binop(int op, Node *lhs, Node *rhs) {
    Node *node = new_node(op);
    node->lhs = lhs;
    node->rhs = rhs;
    return node;
}

Node *new_node_num(int val) {
    Node *node = new_node(ND_NUM);
    node->val = val;
    return node;
}

Node *new_node_ident(char *name) {
    Node *node = new_node(ND_IDENT);
    node->name = name;
    return node;
}

Node *stmt() {
    Node *node;

    if (consume(TK_RETURN)) {
        node = new_node(ND_RETURN);
        node->expr = expr();
        expect(';');
        return node;
    }
    else if (consume(TK_IF)) {
        node = new_node(ND_IF);

        expect('(');
        node->cond = expr();
        expect(')');
        node->then = stmt();

        if (consume(TK_ELSE)) 
            node->els = stmt();
        
        return node;
    }
    else if (consume(TK_WHILE)) {
        node = new_node(ND_WHILE);

        expect('(');
        node->cond = expr();
        expect(')');

        node->body = stmt();
        return node;
    }
    else if (consume(TK_FOR)) {
        node = new_node(ND_FOR);

        expect('(');
        if (!consume(';')) {
            if (is_typename())
                node->init = declaration();
            else
                node->init = expr();
            expect(';');
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
        node = new_node(ND_BLOCK);
        node->stmts = new_vector();

        while (!consume('}')) {
            vec_push(node->stmts, stmt());
        }

        return node;
    }
    else if (is_typename()) {
        node = declaration();
        expect(';');
        return node;
    }
    else {
        /// default statement
        node = expr();
        expect(';');
        return node;
    }
}

Node *expr() {
    return assign();
}

Node *assign() {
    Node *node = ternary();
    if (consume('=')) {
        node = new_node_binop('=', node, assign());
    }
    else if (consume(TK_ADD_EQ)) {
        Node *rhs = new_node_binop('+', node, assign());
        node = new_node_binop('=', node, rhs);
    }
    else if (consume(TK_SUB_EQ)) {
        Node *rhs = new_node_binop('-', node, assign());
        node = new_node_binop('=', node, rhs);
    }
    else if (consume(TK_MUL_EQ)) {
        Node *rhs = new_node_binop('*', node, assign());
        node = new_node_binop('=', node, rhs);
    }
    else if (consume(TK_DIV_EQ)) {
        Node *rhs = new_node_binop('/', node, assign());
        node = new_node_binop('=', node, rhs);
    }
    else if (consume(TK_MOD_EQ)) {
        Node *rhs = new_node_binop('%', node, assign());
        node = new_node_binop('=', node, rhs);
    }
    return node;
}

Node *ternary() {
    Node *node = log_or();
    if (consume('?')) {
        Node *ternary = new_node(ND_IF);
        ternary->cond = node;
        ternary->then = log_or();
        expect(':');
        ternary->els = log_or();
        return ternary;
    }
    return node;
}

Node *log_or() {
    Node *node = log_and();
    for (;;) {
        if (consume(TK_LOG_OR))
            node = new_node_binop(ND_LOG_OR, node, log_or());
        else
            return node;
    }
}

Node *log_and() {
    Node *node = equality();
    for (;;) {
        if (consume(TK_LOG_AND))
            node = new_node_binop(ND_LOG_AND, node, log_and());
        else
            return node;
    }
}

Node *equality() {
    Node *node = relational();
    
    for (;;) {
        if (consume(TK_EQ))
            node = new_node_binop(ND_EQ, node, relational());
        else if (consume(TK_NE))
            node = new_node_binop(ND_NE, node, relational());
        else
            return node;        
    }
}

Node *relational() {
    Node *node = add();
    
    for (;;) {
        if (consume(TK_LE))
            node = new_node_binop(ND_LE, node, add());
        else if (consume(TK_GE))
            node = new_node_binop(ND_LE, add(), node);
        else if (consume('<'))
            node = new_node_binop('<', node, add());
        else if (consume('>'))
            node = new_node_binop('<', add(), node);
        else
            return node;        
    }
}

Node *add() {
    Node *node = mul();

    for (;;) {
        if (consume('+'))
            node = new_node_binop('+', node, mul());
        else if (consume('-'))
            node = new_node_binop('-', node, mul());
        else
            return node;
    }
}

Node *mul() {
    Node *node = unary();

    for (;;) {
        if (consume('*'))
            node = new_node_binop('*', node, unary());
        else if (consume('/'))
            node = new_node_binop('/', node, unary());
        else if (consume('%'))
            node = new_node_binop('%', node, unary());
        else
            return node;
    }
}

Node *unary() {
    if (consume('*')) {
        Node *node = new_node(ND_DEREF);
        node->expr = unary();
        return node;
    }
    if (consume('&')) {
        Node *node = new_node(ND_ADDR);
        node->expr = unary();
        return node;
    }
    if (consume('!')) {
        Node *node = new_node(ND_LOG_NOT);
        node->expr = unary();
        return node;
    }
    if (consume(TK_SIZEOF)) {
        Node *node = new_node(ND_SIZEOF);
        node->expr = unary();
        return node;
    }
    if (consume('+'))
        return term();
    if (consume('-'))
        return new_node_binop('-', new_node_num(0), term());
    
    // pre-increment/decrement
    if (consume(TK_INC)) {
        Node *lhs = term();
        Node *rhs = new_node_binop('+', lhs, new_node_num(1));
        return new_node_binop('=', lhs, rhs);
    }
    if (consume(TK_DEC)) {
        Node *lhs = term();
        Node *rhs = new_node_binop('-', lhs, new_node_num(1));
        return new_node_binop('=', lhs, rhs);
    }
    return postfix();
}

Node *postfix() {
    Node *lhs = term();

    // Array
    while(consume('[')) {
        lhs = new_node_expr(ND_DEREF, new_node_binop('+', lhs, add()));
        expect(']');
    }

    // Post increment/decrement
    Token *t = tokens->data[pos];
    if (t->ty == TK_INC)
    {
        pos++;
        lhs = new_node_expr(ND_POST_INC, lhs);
        return lhs;
    }
    else if (t->ty == TK_DEC)
    {
        pos++;
        lhs = new_node_expr(ND_POST_DEC, lhs);
        return lhs;
    }

    return lhs;
}

Node *term() {
    Token *t = tokens->data[pos];
    if (consume('(')) {
        Node *node = expr();
        expect(')');
        return node;
    }

    // For sizeof 
    if (is_typename()) {
        pos++;
        int size = sizeof_types(t->ty);
        Node *node = new_node(ND_TY_SIZE);
        node->val = size;
        return node;
    }

    if (t->ty == TK_NUM) {
        pos++;
        return new_node_num(t->val);
    }

    // String literal
    if (t->ty == TK_STRING) {
        pos++;
        Type *ty = char_ty();
        ty = arr_ty(ty, t->len);

        char *str_label = format(".LSTR%d", str_count++);
        add_gvar(ty, str_label, t->name);
        return new_node_ident(str_label);
    }

    if (t->ty == TK_IDENT) {
        pos++;
        Node *node;

        // Identifier
        if (!consume('(')) {
            if (map_get(vars, t->name) == NULL) {
                if (map_get(prog->gvars, t->name) == NULL)
                    error_at(t->input, "undefined variable");
            }
            return new_node_ident(t->name);
        }

        // Function call
        node = new_node(ND_CALL);
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
    Type *ty = type_specifier();

    Token *t = tokens->data[pos];
    if (!consume(TK_IDENT))
        error_at(t->input, "not variable declaration");

    Node *node = new_node(ND_VARDEF);
    node->name = t->name;

    if (map_get(vars, node->name) != NULL)
        error("'%s' is already defined", node->name);

    // array check
    ty = arr_of(ty);

    // variable setting
    Var *var = malloc(sizeof(Var));
    offset += ty->size;
    var->offset = offset;
    var->ty = ty;
    var->name = t->name;

    node->ty = ty;

    map_put(vars, node->name, var);

    node->init = NULL;
    if (consume('=')) {
        node->init = assign();
    }

    return node;
}

Node *param()
{
    Type *ty = type_specifier();

    Token *t = tokens->data[pos];
    if (!consume(TK_IDENT))
        error_at(t->input, "not variable declaration");

    // array check
    ty = arr_of(ty);

    // variable setting
    Var *var = malloc(sizeof(Var));
    offset += ty->size;
    var->offset = offset;
    var->ty = ty;
    var->name = t->name;

    Node *node = new_node(ND_VARDEF);
    node->name = t->name;
    node->ty = ty;

    map_put(vars, node->name, var);

    return node;
}

Function *function(Type *ty, char *name) {

    // Token *t = tokens->data[pos++];

    Node *node = new_node(ND_FUNC);
    node->ty = ty;
    node->name = name;
    node->args = new_vector();

    Function *fn = malloc(sizeof(Function));
    fn->node = node;
    fn->ty = ty;

    // make space for local variables every time in function definition
    vars = new_map();
    offset = 0;
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

void add_gvar(Type *ty, char *name, char *data) {
    Var *v = malloc(sizeof(Var));
    v->ty = ty;
    v->name = name;
    v->data = data;
    map_put(prog->gvars, name, v);
}

void toplevel() {

    while (!peek(TK_EOF)) {

        Type *ty = type_specifier();

        Token *t = tokens->data[pos];
        expect(TK_IDENT);

        // Function
        if (peek('('))
            vec_push(prog->fns, function(ty, t->name));
        // Global variable
        else {
            ty = arr_of(ty);
            add_gvar(ty, t->name, 0);
            expect(';');
        }
    }
    vec_push(prog->fns, NULL);
}

Program *parse(Vector *v) {
    tokens = v;
    pos = 0;

    prog = malloc(sizeof(Program));
    prog->gvars = new_map();
    prog->fns = new_vector();

    toplevel();

    return prog;
}