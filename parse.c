#include "xcc.h"

static Vector *tokens;
static int pos;
static Program *prog;

static Vector *lvars;
static int offset;
static int str_count;

static Vector *switches;
static Vector *breaks;
static Vector *continues;


static Env *env;
static Env *new_env(Env *prev) {
    Env *ret = calloc(1, sizeof(Env));
    ret->vars = new_map();
    ret->prev = prev;
    return ret;
}

Var *add_lvar(Type *ty, char *name) {
    Var *v = calloc(1, sizeof(Var));
    v->ty = ty;
    v->name = name;
    v->is_local = 1;

    offset += ty->size;
    v->offset = roundup(offset, ty->align);

    map_put(env->vars, name, v);
    vec_push(lvars, v);
}

Var *add_gvar(Type *ty, char *name, char *data, int is_extern) {
    Var *v = calloc(1, sizeof(Var));
    v->ty = ty;
    v->name = name;
    v->is_local = 0;
    v->data = data;
    v->is_extern = is_extern;
    map_put(prog->gvars, name, v);
}

static Var *find_var(char *name) {
    for (Env *cur = env; cur; cur = cur->prev) {
        Var *var = map_get(cur->vars, name);
        if (var)
            return var;
    }
    error("undefined variable, %s", name);
    return NULL;
}


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
    return t->ty == TK_INT || t->ty == TK_CHAR || t->ty == TK_STRUCT;
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
    Type *ty;

    Token *t = tokens->data[pos++];
    switch (t->ty) {
        case TK_CHAR:
            ty = char_ty();
            break;
        case TK_INT:
            ty = int_ty();
            break;
    }

    if (t->ty == TK_STRUCT) {
        // TODO: refactor inside this 'if' to separate function
        ty = new_ty(STRUCT, 1);
        ty->members = new_map();
        // TODO: check struct tag

        expect('{');
        int off = 0;
        while (!consume('}')) {
            Node *node = declaration();
            consume(';');

            Type *t = node->ty;
            map_put(ty->members, node->name, t);

            off = roundup(off, t->align);
            off += t->size;
            t->offset = off;

            // struct alignment is the same as its largest member's align
            if (t->align > ty->align)
                ty->align = t->align;
        }

        /* For alingment check */
        // for (int i = 0; i < ty->members->keys->len; i++) {
        //     Type *t = ty->members->vals->data[i];
        //     printf("%s, %d\n", ty->members->keys->data[i], t->offset);
        // }

        ty->size = roundup(off, ty->align);
        return ty;
    }

    // TODO: move pointer check to outside of this function
    while (consume('*')) {
        ty = ptr_to(ty);
    }

    return ty;
}

Type *ptr_to(Type *base) {
    Type *new_ty = calloc(1, sizeof(Type));
    new_ty->ty = PTR;
    new_ty->size = 8;
    new_ty->align = 8;
    new_ty->ptr_to = base;
    return new_ty;
}

static int *const_expr() {
    Token *t = tokens->data[pos];
    Node *node = expr();
    if (node->op != ND_NUM)
        error("constant expression expected");
    return node->val;
}

static Type *arr_of(Type *base) {
    Type *ret = base;
    Vector *stack = new_vector();

    while (consume('[')) {
        if (consume(']')) {
            vec_push(stack, (void *)0);
            continue;
        }
        vec_push(stack, (void *)const_expr());
        expect(']');
    }

    for (int i = stack->len - 1; i >= 0; i--) {
        Type *new_ty = calloc(1, sizeof(Type));
        new_ty->ty = ARRAY;        
        new_ty->array_size = stack->data[i];
        new_ty->size = new_ty->array_size * ret->size;
        new_ty->align = base->align;
        new_ty->arr_of = ret;
        new_ty->ptr_to = ret;
        ret = new_ty;
    }

    return ret;
}

Node *new_node(int op) {
    Node *node = calloc(1, sizeof(Node));
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

Node *new_node_vardef(Var *var) {
    Node *node = new_node(ND_VARDEF);
    node->name = var->name;
    node->ty = var->ty;
    node->var = var;
    return node;
}

Node *string_literal(Token *t) {
    char *str_label = format(".LSTR%d", str_count++);
    Node *node = new_node_ident(str_label);

    Type *ty = arr_ty(char_ty(), t->len + 1);    // +1 means null terminating character
    Var *var = add_gvar(ty, str_label, t->name, 0);

    node->var = var;
    node->ty = ty;
    return node;
}

Node *local_variable(Token *t) {
    Var *var = find_var(t->name);
    Node *node = new_node_ident(t->name);
    node->ty = var->ty;
    node->var = var;
    return node;
}

Node *function_call(Token *t) {
    Node *node = new_node(ND_CALL);
    node->name = t->name;
    node->args = new_vector();
    node->ty = int_ty(); // TODO: specify accurate type for function call

    if (consume(')'))
        return node;

    do {
        vec_push(node->args, expr());
    } while (consume(','));
    expect(')');

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
        node = new_node(ND_FOR);
        vec_push(breaks, node);
        vec_push(continues, node);

        expect('(');
        node->cond = expr();
        expect(')');
        node->body = stmt();

        vec_pop(breaks);
        vec_pop(continues);
        return node;
    }
    else if (consume(TK_DO_WHILE)) {
        node = new_node(ND_DO_WHILE);
        vec_push(breaks, node);
        vec_push(continues, node);

        node->body = stmt();
        expect(TK_WHILE);
        expect('(');
        node->cond = expr();
        expect(')');
        expect(';');

        vec_pop(breaks);
        vec_pop(continues);
        return node;
    }
    else if (consume(TK_FOR)) {
        node = new_node(ND_FOR);
        vec_push(breaks, node);
        vec_push(continues, node);

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

        vec_pop(breaks);
        vec_pop(continues);
        return node;
    }
    else if (consume(TK_SWITCH)) {
        node = new_node(ND_SWITCH);
        vec_push(switches, node);
        vec_push(breaks, node);
        node->cases = new_vector();

        expect('(');
        node->cond = expr();
        expect(')');
        node->body = stmt();

        vec_pop(switches);
        vec_pop(breaks);
        return node;
    }
    else if (consume(TK_CASE)) {
        node = new_node(ND_CASE);
        node->val = const_expr();
        expect(':');
        node->body = stmt();

        Node *n = vec_top(switches);
        vec_push(n->cases, node);
        return node;
    }
    else if (consume('{')) {
        node = new_node(ND_BLOCK);
        node->stmts = new_vector();
        env = new_env(env);

        while (!consume('}')) {
            vec_push(node->stmts, stmt());
        }

        env = env->prev;
        return node;
    }
    else if (consume(TK_BREAK)) {
        node = new_node(ND_BREAK);
        node->target = vec_top(breaks);
        expect(';');
        return node;
    }
    else if (consume(TK_CONTINUE)) {
        node =  new_node(ND_CONTINUE);
        node->target = vec_top(continues);
        expect(';');
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
    Node *node = bit_or();
    for (;;) {
        if (consume(TK_LOG_AND))
            node = new_node_binop(ND_LOG_AND, node, log_and());
        else
            return node;
    }
}

Node *bit_or() {
    Node *node = bit_xor();
    for (;;) {
        if (consume('|'))
            node = new_node_binop('|', node, bit_or());
        else
            return node;
    }
}

Node *bit_xor() {
    Node *node = bit_and();
    for (;;) {
        if (consume('^'))
            node = new_node_binop('^', node, bit_xor());
        else
            return node;
    }
}

Node *bit_and() {
    Node *node = equality();
    for (;;) {
        if (consume('&'))
            node = new_node_binop('&', node, equality());
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
    if (consume('~')) {
        Node *node = new_node('~');
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
    if (t->ty == '(') {
        pos++;
        Node *node;
        if (peek('{'))
            node = stmt();
        else
            node = expr();
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

    // Ascii character
    if (t->ty == TK_ASCII) {
        pos++;
        Node *node = new_node_num(t->val);
        node->ty = char_ty();
        return node;
    }

    if (t->ty == TK_STRING) {
        pos++;
        return string_literal(t);
    }

    if (t->ty == TK_IDENT) {
        pos++;
        if (!consume('(')) {
            return local_variable(t);
        }
        return function_call(t);
    }

    // error("%s is invalid", t->input);
    error_at(t->input, "non-number or opening parentheses Token found");

    return NULL;
}

Node *declaration() {
    Type *ty = type_specifier();

    Token *t = tokens->data[pos];
    if (!consume(TK_IDENT))
        error_at(t->input, "not variable declaration");


    // array check
    ty = arr_of(ty);

    Var *var = add_lvar(ty, t->name);

    Node *node = new_node_vardef(var);
    if (consume('=')) {
        node->init = expr();
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
    if (ty->ty == ARRAY)
        ty = ptr_to(ty->arr_of);

    // variable setting
    Var *var = add_lvar(ty, t->name);

    Node *node = new_node_vardef(var);
    return node;
}

Vector *params() {
    Vector *params = new_vector();
    expect('(');
    if (!consume(')')) {
        vec_push(params, param());
        while (consume(',')) {
            vec_push(params, param());
        }
        expect(')');
    }
    return params;
}

Function *function(Type *ty, char *name) {
    Node *node = new_node(ND_FUNC);
    node->ty = ty;
    node->name = name;

    Function *fn = calloc(1, sizeof(Function));
    fn->node = node;
    fn->ty = ty;
    fn->lvars = new_vector();
    lvars = fn->lvars;
    // reset offset
    offset = 0;

    env = new_env(env);
    node->args = params();
    node->body = stmt();
    env = env->prev;
    return fn;
}


void toplevel() {

    while (!peek(TK_EOF)) {
        int is_extern = consume(TK_EXTERN);

        Type *ty = type_specifier();

        Token *t = tokens->data[pos];
        expect(TK_IDENT);

        // Function
        if (peek('('))
            vec_push(prog->fns, function(ty, t->name));
        // Global variable
        else {
            ty = arr_of(ty);
            add_gvar(ty, t->name, 0, is_extern);
            expect(';');
        }
    }
    vec_push(prog->fns, NULL);
}

Program *parse(Vector *v) {
    tokens = v;
    pos = 0;
    switches = new_vector();
    breaks = new_vector();
    continues = new_vector();

    prog = calloc(1, sizeof(Program));
    prog->gvars = new_map();
    env = new_env(NULL);
    env->vars = prog->gvars;
    prog->fns = new_vector();

    toplevel();

    return prog;
}