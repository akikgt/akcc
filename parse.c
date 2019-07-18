#include "xcc.h"

static Vector *tokens;
static int pos;
static Program *prog;

static Function *cur_fn;
static Vector *lvars;
static int g_offset;
static int str_count;

static Vector *switches;
static Vector *breaks;
static Vector *continues;

Node *declaration_type();


static Env *env;
static Env *new_env(Env *prev) {
    Env *ret = calloc(1, sizeof(Env));
    ret->vars = new_map();
    ret->tags = new_map();
    ret->typedefs = new_map();
    ret->enums = new_map();
    ret->prev = prev;
    return ret;
}

static Var *new_var(Type *ty, char *name, int is_local) {
    Var *v = calloc(1, sizeof(Var));
    v->ty = ty;
    v->name = name;
    v->is_local = is_local;
    return v;
}

Var *add_lvar(Type *ty, char *name) {
    Var *v = new_var(ty, name, 1);

    g_offset += ty->size;
    v->offset = roundup(g_offset, ty->align);
    g_offset = v->offset;

    map_put(env->vars, name, v);
    vec_push(lvars, v);
}

Var *add_gvar(Type *ty, char *name, char *data, int is_extern) {
    Var *v = new_var(ty, name, 0);
    v->str_data = data;
    v->is_extern = is_extern;
    map_put(prog->gvars, name, v);
}


static Var *find_var(char *name) {
    for (Env *cur = env; cur; cur = cur->prev) {
        Var *var = map_get(cur->vars, name);
        if (var)
            return var;
    }
    // error("undefined variable, %s", name);
    printf("undefined variable, %s", name);
    exit(1);
    return NULL;
}

static Type *find_tag(char *name) {
    for (Env *cur = env; cur; cur = cur->prev) {
        Type *ty = map_get(cur->tags, name);
        if (ty)
            return ty;
    }
    return NULL;
}

static Type *find_enum(char *name) {
    for (Env *cur = env; cur; cur = cur->prev) {
        Type *ty = map_get(cur->enums, name);
        if (ty)
            return ty;
    }
    return NULL;
}

static Type *find_typedef(char *name) {
    for (Env *cur = env; cur; cur = cur->prev) {
        Type *ty = map_get(cur->typedefs, name);
        if (ty)
            return ty;
    }
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
    if (t->ty == TK_IDENT)
        return find_typedef(t->name);
    return t->ty == TK_INT || t->ty == TK_CHAR || t->ty == TK_STRUCT
            || t->ty == TK_VOID || t->ty == TK_ENUM;
}

static char *ident() {
    Token *t = tokens->data[pos];
    if (t->ty != TK_IDENT)
        error_at(t->input, "Identifier expected");
    pos++;
    return t->name;
}

static int numeric() {
    Token *t = tokens->data[pos];
    if (t->ty != TK_NUM)
        error_at(t->input, "Numeric value expected");
    pos++;
    return t->val;
}

static Type *type_specifier() {

    Token *t = tokens->data[pos++];
    switch (t->ty) {
        case TK_VOID:
            return void_ty();
        case TK_CHAR:
            return char_ty();
        case TK_INT:
            return int_ty();
        case TK_IDENT: {
            Type *ret = find_typedef(t->name);
            if (!ret)
                error_at(t->input, "unknown type name");
            return ret;
        }
    }

    if (t->ty == TK_ENUM) {
        Token *t = tokens->data[pos];
        char *tag_name = NULL;
        Type *ty = NULL;

        if (t->ty == TK_IDENT) {
            pos++;
            tag_name = t->name;
            ty = find_tag(tag_name);
        }

        // tag is not defined yet
        if (ty)
            return ty;

        // consider enum as int
        if (tag_name)
            map_put(env->tags, t->name, int_ty());

        int val = 0;
        expect('{');
        do {
            if (consume('}'))
                break;
            char *name = ident();
            if (consume('='))
                val = numeric();
            ty = enum_ty(val);
            map_put(env->enums, name, ty);
            val++;
        } while (consume(',') || !consume('}'));


        // consider enum as int
        return int_ty();
    }

    if (t->ty == TK_STRUCT) {
        Token *t = tokens->data[pos];
        char *tag_name = NULL;
        Type *ty = NULL;
        if (t->ty == TK_IDENT) {
            pos++;
            tag_name = t->name;
            ty = find_tag(tag_name);
        }

        // tag has already been defined
        if (ty)
            return ty;

        ty = new_ty(STRUCT, 1);
        ty->members = new_map();

        // For struct which has itself as member, save the tag_name and type first
        if (tag_name)
            map_put(env->tags, tag_name, ty);

        expect('{');
        int off = 0;
        while (!consume('}'))
        {
            // TODO: make this block to the function
            Node *node = declaration_type();

            Type *t = node->ty;
            map_put(ty->members, node->name, t);

            off = roundup(off, t->align);
            t->offset = off;
            off += t->size;

            // struct alignment is the same as its largest member's align
            if (t->align > ty->align)
                ty->align = t->align;
        }

        ty->size = roundup(off, ty->align);

        return ty;
    }

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

    if (!stack->len)
        return ret;

    for (int i = stack->len - 1;;i--) {
        Type *new_ty = calloc(1, sizeof(Type));
        new_ty->ty = ARRAY;        
        new_ty->array_size = stack->data[i];
        new_ty->size = new_ty->array_size * ret->size;
        new_ty->align = base->align;
        new_ty->arr_of = ret;
        new_ty->ptr_to = ret;
        ret = new_ty;

        if (i == 0)
            break;
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

// TODO: delete it and integrate with new_node_varref
Node *new_node_ident(char *name) {
    Node *node = new_node(ND_IDENT);
    node->name = name;
    node->can_be_lval = 1;
    return node;
}

Node *new_node_varref(Var *var) {
    Node *node = new_node(ND_IDENT);
    node->name = var->name;
    node->var = var;
    node->ty = var->ty;
    node->can_be_lval = 1;
    return node;
}

Node *new_node_vardef(Var *var) {
    Node *node = new_node(ND_VARDEF);
    node->name = var->name;
    node->ty = var->ty;
    node->var = var;
    node->can_be_lval = 1;
    return node;
}

// replace 'x op=y' with 'Type *z = x; *z = *z op + y'
Node *new_node_assign_eq(int op, Node *lhs, Node *rhs) {
    Node *node = new_node(ND_BLOCK);
    node->stmts = new_vector();

    // Type *z = x;
    Var *var = add_lvar(ptr_to(lhs->ty), "tmp");
    vec_push(node->stmts, new_node_binop('=', new_node_varref(var), new_node_expr(ND_ADDR, lhs)));

    // *z = *z op y;
    vec_push(node->stmts, new_node_binop('=', new_node_expr(ND_DEREF, new_node_varref(var)),
        new_node_binop(op, new_node_expr(ND_DEREF, new_node_varref(var)), rhs)));

    node->ty = lhs->ty;
    return node;
}

Var *add_str(Token *t) {
    // add string to the .data section
    char *str_label = format(".LSTR%d", str_count++);
    Type *ty = arr_ty(char_ty(), t->len + 1); // +1 means null terminating character
    Var *var = add_gvar(ty, str_label, t->name, 0);
    var->has_init = 1;
    return var;
}

Node *string_literal(Token *t) {
    Var *var = add_str(t);
    Node *node = new_node_ident(var->name);
    node->var = var;
    node->ty = var->ty;
    return node;
}

Node *local_variable(Token *t) {
    Type *ty = find_enum(t->name);
    if (ty)
        return new_node_num(ty->val);

    Var *var = find_var(t->name);
    Node *node = new_node_varref(var);
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
        vec_push(node->args, assign());
    } while (consume(','));
    expect(')');

    return node;
}

Node *variadic_function() {
    // for va_start
    Node *node = new_node(ND_VA_START);
    node->stmts = new_vector();
    node->fn = cur_fn;
    char *ap = ident();

    Var *var = find_var(ap);
    if (var->ty->ty != ARRAY || var->ty->arr_of->ty != STRUCT)
        error("va_start requires array of struct");

    Node *gp_offset = new_node_expr(ND_DOT, new_node_expr(ND_DEREF, new_node_varref(var)));
    gp_offset->name = "gp_offset";
    gp_offset = new_node_binop('=', gp_offset, new_node_num(cur_fn->arity * 8));
    vec_push(node->stmts, gp_offset);

    Node *fp_offset = new_node_expr(ND_DOT, new_node_expr(ND_DEREF, new_node_varref(var)));
    fp_offset->name = "fp_offset";
    fp_offset = new_node_binop('=', fp_offset, new_node_num(48));
    vec_push(node->stmts, fp_offset);

    Node *overflow_area = new_node_expr(ND_DOT, new_node_expr(ND_DEREF, new_node_varref(var)));
    overflow_area->name = "overflow_arg_area";
    overflow_area = new_node_expr(ND_ADDR, overflow_area);
    vec_push(node->stmts, overflow_area);

    Node *reg_save_area = new_node_expr(ND_DOT, new_node_expr(ND_DEREF, new_node_varref(var)));
    reg_save_area->name = "reg_save_area";
    reg_save_area = new_node_expr(ND_ADDR, reg_save_area);
    vec_push(node->stmts, reg_save_area);


    consume(',');
    consume(TK_IDENT);
    expect(')');
    return node;
}

Node *stmt() {
    Node *node;

    if (consume(TK_RETURN)) {
        node = new_node(ND_RETURN);
        if (!consume(';')) {
            node->expr = expr();
            expect(';');
        }
        return node;
    }
    else if (consume(TK_TYPEDEF)) {
        Type *ty = type_specifier();
        while (consume('*'))
            ty = ptr_to(ty);
        char *name = ident();
        ty = arr_of(ty);
        map_put(env->typedefs, name, ty);
        expect(';');
        // TODO: make void node
        return new_node_num(0);
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
        if (is_typename())
            node->init = declaration();
        else if (!consume(';')) {
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
    else if (consume(TK_DEFAULT)) {
        node = new_node(ND_DEFAULT);
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
        return node;
    }
    else if (consume(TK_STATIC) || consume(TK_CONST)) {
        // skip static or const
        return stmt();
    }
    else {
        /// default statement
        node = expr();
        expect(';');
        return node;
    }
}

Node *expr() {
    Node *lhs = assign();
    while (consume(','))
        lhs = new_node_binop(',', lhs, assign());
    return lhs;
}

Node *assign() {
    Node *node = ternary();
    if (consume('=')) {
        node = new_node_binop('=', node, assign());
    }
    else if (consume(TK_ADD_EQ)) {
        node = new_node_assign_eq('+', node, assign());
    }
    else if (consume(TK_SUB_EQ)) {
        node = new_node_assign_eq('-', node, assign());
    }
    else if (consume(TK_MUL_EQ)) {
        node = new_node_assign_eq('*', node, assign());
    }
    else if (consume(TK_DIV_EQ)) {
        node = new_node_assign_eq('/', node, assign());
    }
    else if (consume(TK_MOD_EQ)) {
        node = new_node_assign_eq('%', node, assign());
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
    Node *node = shift();
    
    for (;;) {
        if (consume(TK_LE))
            node = new_node_binop(ND_LE, node, shift());
        else if (consume(TK_GE))
            node = new_node_binop(ND_LE, shift(), node);
        else if (consume('<'))
            node = new_node_binop('<', node, shift());
        else if (consume('>'))
            node = new_node_binop('<', shift(), node);
        else
            return node;        
    }
}

Node *shift() {
    Node *node = add();

    for (;;) {
        if (consume(TK_SHL))
            node = new_node_binop(ND_SHL, node, add());
        else if (consume(TK_SHR))
            node = new_node_binop(ND_SHR, node, add());
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
        if (consume('(')) {
            if (is_typename()) {
                Type *ty = type_specifier();
                while(consume('*'))
                    ty = ptr_to(ty);
                node->expr = new_node(ND_TY_SIZE);
                node->expr->val = ty->size;
                expect(')');
                return node;
            }
            // undo consume('(')
            pos--;
        }
        node->expr = unary();
        return node;
    }
    // cast
    if (consume('(')) {
        if (!is_typename()) {
            pos--;
            return postfix();
        }
        Type *ty = type_specifier();
        while (consume('*'))
            ty = ptr_to(ty);
        expect(')');
        Node *node = new_node(ND_CAST);
        node->ty = ty;
        node->expr = unary();
        return node;
    }
    if (consume('+'))
        return postfix();
    if (consume('-'))
        return new_node_binop('-', new_node_num(0), postfix());
    // pre-increment/decrement
    if (consume(TK_INC)) {
        return new_node_assign_eq('+', postfix(), new_node_num(1));
    }
    if (consume(TK_DEC)) {
        return new_node_assign_eq('-', postfix(), new_node_num(1));
    }
    return postfix();
}

Node *postfix() {
    Node *lhs = term();

    for (;;) {
        Token *t = tokens->data[pos];

        // Post increment/decrement
        if (consume(TK_INC)) {
            lhs = new_node_expr(ND_POST_INC, lhs);
            continue;
        }
        if (consume(TK_DEC)) {
            lhs = new_node_expr(ND_POST_DEC, lhs);
            continue;
        }

        // dot operaotr for struct
        if (consume('.')) {
            lhs = new_node_expr(ND_DOT, lhs);
            t = tokens->data[pos++];
            lhs->name = t->name;
            continue;
        }

        if (consume(TK_ARROW)) {
            lhs = new_node_expr(ND_DOT, new_node_expr(ND_DEREF, lhs));
            t = tokens->data[pos++];
            lhs->name = t->name;
            continue;
        }

        // Array
        if (consume('[')) {
            char *arr_name = lhs->name;
            lhs = new_node_expr(ND_DEREF, new_node_binop('+', lhs, add()));
            lhs->name = arr_name;
            expect(']');
            continue;
        }

        return lhs;
    }

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
        else if (!strcmp(t->name, "__builtin_va_start")) {
            return variadic_function();
        }
        else
            return function_call(t);
    }

    error_at(t->input, "non-number or opening parentheses Token found");

    return NULL;
}

Node *declaration() {
    Type *ty = type_specifier();
    // TODO: move pointer check to outside of this function
    while (consume('*')) {
        ty = ptr_to(ty);
    }

    Token *t = tokens->data[pos];
    if (consume(';')) {
        // when just declare tag like struct, do nothing
        // TODO: make new_node_void or new_node_nop
        return new_node_num(0);
    }
    else if (!consume(TK_IDENT))
        error_at(t->input, "not variable declaration");

    // array check
    ty = arr_of(ty);

    Var *var = add_lvar(ty, t->name);

    Node *node = new_node_vardef(var);
    if (consume('=')) {
        node->init = expr();
    }

    expect(';');
    return node;
}

Node *declaration_type() {
    Type *ty = type_specifier();
    // TODO: move pointer check to outside of this function
    while (consume('*')) {
        ty = ptr_to(ty);
    }
    Token *t = tokens->data[pos++];

    // TODO declarator
    // array check
    ty = arr_of(ty);

    Node *node = new_node(ND_VARDEF);
    node->ty = ty;
    if (t->ty == TK_IDENT)
        node->name = t->name;

    expect(';');
    return node;
}

Node *param()
{
    if (consume(TK_VARARGS)) {
        cur_fn->is_variadic = 1;
        return new_node(ND_VARARGS);
    }

    // just skip const. TODO for later
    consume(TK_CONST);

    Type *ty = type_specifier();
    // TODO: move pointer check to outside of this function
    while (consume('*')) {
        ty = ptr_to(ty);
    }

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
    cur_fn->arity++;
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
    // TODO: save function names already defined
    Node *node = new_node(ND_FUNC);
    node->ty = ty;
    node->name = name;

    Function *fn = calloc(1, sizeof(Function));
    cur_fn = fn;
    fn->node = node;
    fn->ty = ty;
    fn->lvars = new_vector();
    lvars = fn->lvars;
    // reset offset
    g_offset = 0;

    env = new_env(env);
    node->args = params();
    if (!consume(';'))
        node->body = stmt();
    else
        fn = NULL; // if there is no body, this function is just prototype.
    env = env->prev;
    return fn;
}

void gvar_init(Var *gv) {
    gv->has_init = 1;
    Type *ty = gv->ty;
    if (ty->ty != ARRAY && ty->ty != STRUCT) {
        gv->init_val = numeric();
        return;
    }

    expect('{');
    // array initialization
    gv->arr_data = new_vector();
    do {
        Token *t = tokens->data[pos];
        Var *var;
        if (t->ty == TK_STRING) {
            pos++;
            var = add_str(t);
        }
        else if (t->ty == TK_NUM) {
            var = calloc(1, sizeof(Var));
            var->init_val = numeric();
            var->ty = gv->ty->arr_of;
        }
        vec_push(gv->arr_data, var);
    } while (consume(','));

    expect('}');
}

void toplevel() {
    while (!peek(TK_EOF)) {
        int is_extern = consume(TK_EXTERN);
        int is_static = consume(TK_STATIC);
        int is_const = consume(TK_CONST);
        int is_typedef = consume(TK_TYPEDEF);

        Type *ty = type_specifier();
        // TODO: move pointer check to outside of this function
        while (consume('*')) {
            ty = ptr_to(ty);
        }

        // For struct, union, enum without identifier
        if (consume(';'))
            continue;

        char *name = ident();
        // Function
        if (peek('(')) {
            Function *fn = function(ty, name);
            // fn == NULL means, fn is just prototype of the function
            // TODO: if fn is prototype, only save its name and parameters
            if (fn)
                vec_push(prog->fns, fn);
        }
        // Global variable
        else {
            ty = arr_of(ty);
            if (is_typedef) {
                map_put(env->typedefs, name, ty);
                expect(';');
                continue;
            }
            Var *gv = add_gvar(ty, name, 0, is_extern);
            gv->is_static = is_static;
            gv->is_const = is_const;
            if (consume('=')) {
                gvar_init(gv);
            }
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