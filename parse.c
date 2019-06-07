#include "xcc.h"

static Vector *tokens;
static int pos;
static Vector *code;
static int var_count;
// Map *map;

int consume(int ty) {
    Token *t = tokens->data[pos];
    if (t->ty != ty)
        return 0;
    pos++;
    return 1;
}

// static void expect(int ty) {
//     if (!consume(ty))
//         error_at(((Token *)tokens->data[pos])->input, "Not expected");
//         printf("%c\n", ty);
// }

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

    // if (map_get(map, name) == NULL) {
    //     int offset = (var_count + 1) * 8;
    //     map_put(map, name, (void *)offset);
    //     var_count++;
    // }

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
        if (!consume(';'))
            error_at(((Token *)tokens->data[pos])->input, "Not ';'");
        return node;
    }
    else if (consume(TK_IF)) {
        node = malloc(sizeof(Node));
        node->ty = ND_IF;

        if (!consume('('))
            error_at(((Token *)tokens->data[pos])->input, "Not '('");
        node->cond = expr();
        if (!consume(')'))
            error_at(((Token *)tokens->data[pos])->input, "missing closing parentheses");
        node->then = stmt();

        if (consume(TK_ELSE)) 
            node->els = stmt();
        
        return node;
    }
    else if (consume(TK_WHILE)) {
        node = malloc(sizeof(Node));
        node->ty = ND_WHILE;

        if (!consume('('))
            error_at(((Token *)tokens->data[pos])->input, "Not '('");
        node->cond = expr();
        if (!consume(')'))
            error_at(((Token *)tokens->data[pos])->input, "missing closing parentheses");

        node->then = stmt();
        return node;
    }
    else if (consume(TK_FOR)) {
        node = malloc(sizeof(Node));
        node->ty = ND_FOR;

        if (!consume('('))
            error_at(((Token *)tokens->data[pos])->input, "Not '('");

        if (!consume(';')) {
            node->init = expr();
            if (!consume(';'))
                error_at(((Token *)tokens->data[pos])->input, "Not ';'");
        }

        if (!consume(';')) {
            node->cond = expr();
            if (!consume(';'))
                error_at(((Token *)tokens->data[pos])->input, "Not ';'");
        }

        if (!consume(')')) {
            node->inc = expr();
            if (!consume(')'))
                error_at(((Token *)tokens->data[pos])->input, "missing closing parentheses");
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
    else {
        /// default statement
        node = expr();
        if (!consume(';'))
            error_at(((Token *)tokens->data[pos])->input, "Not ';'");
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
        if (consume('+'))
            node = new_node('+', node, mul());
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
        if (!consume(')'))
            error_at(t->input, "missing closing parentheses");
        return node;
    }

    if (t->ty == TK_NUM) {
        pos++;
        return new_node_num(t->val);
    }

    if (t->ty == TK_IDENT) {
        pos++;
        Node *node;

        // identifier
        if (!consume('(')) {
            node = new_node_ident(t->name);
            return node;
        }

        // function call
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
        if (!consume(')'))
            error_at(t->input, "missing closing parentheses");

        return node;
    }

    error_at(t->input, "non-number or opening parentheses Token found");

    return NULL;
}

Node *function() {
    Node *node = stmt();
    return node;
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

    // map = new_map();
    // var_count = 0;

    program();
    return code;
}