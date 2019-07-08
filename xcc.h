#define _GNU_SOURCE
#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

extern char *user_input;

// vector
typedef struct {
    void **data;
    int capacity;
    int len;
} Vector;

Vector *new_vector();
void vec_push(Vector *vec, void *elem);
void *vec_pop(Vector *vec);
void *vec_top(Vector *vec);
void runtest();

// map
typedef struct {
    Vector *keys;
    Vector *vals;
} Map;

Map *new_map();
void map_put(Map *map, char *key, void *val);
void *map_get(Map *map, char *key);
void test_map();

/// type
typedef struct Type {
    enum { INT, CHAR, PTR, ARRAY, STRUCT } ty;
    int size;
    int align;

    // Pointer
    struct Type *ptr_to;

    // Array
    struct Type *arr_of;
    int array_size;  // TODO: use size_t

    // Struct
    Map *members;
    int offset;
} Type;

/// variable
typedef struct Var {
    Type *ty;
    int offset;
    char *name;
    int is_local;

    // for global variable initial data
    char *data;

    // extern
    int is_extern;

} Var;

/// util.c
char *format(char *fmt, ...);
char *read_file(char *path);
Type *new_ty(int ty, int size);
Type *int_ty();
Type *char_ty();
Type *arr_ty(Type *base, int len);
int roundup(int x, int align); 



/// token.c
enum
{
    TK_NUM = 256, // integers
    TK_STRING,    // string literal
    TK_ASCII,     // ascii character
    TK_IDENT,     // identifier
    TK_SIZEOF,    // sizeof
    TK_RETURN,    // return
    TK_IF,        // if
    TK_ELSE,      // else
    TK_WHILE,     // while
    TK_DO_WHILE,  // do while
    TK_FOR,       // for
    TK_SWITCH,    // switch
    TK_CASE,      // case
    TK_BREAK,     // break
    TK_CONTINUE,  // continue
    TK_EXTERN,    // extern
    TK_STRUCT,    // struct
    TK_INT,       // int
    TK_CHAR,      // char
    TK_ARROW,     // ->
    TK_EQ,        // ==
    TK_NE,        // !=
    TK_LE,        // <=
    TK_GE,        // >=
    TK_ADD_EQ,    // +=
    TK_SUB_EQ,    // -=
    TK_MUL_EQ,    // *=
    TK_DIV_EQ,    // /=
    TK_MOD_EQ,    // %=
    TK_INC,       // increment
    TK_DEC,       // decrement
    TK_LOG_AND,   // &&
    TK_LOG_OR,    // ||
    TK_EOF,       // EOF
};

typedef struct {
    int ty;         // type of token
    int val;        // the value when ty == TK_NUM
    char *name;     // name of identifier when ty == TK_IDENT

    // string literal
    int len;

    char *input;    // token strings for error message
} Token;

Vector *tokenize(char *p);


/// parse.c
enum
{
    ND_NUM = 256, // integers
    ND_VARDEF,    // variable definition
    ND_IDENT,     // identifier
    ND_SIZEOF,    // sizeof
    ND_RETURN,    // return
    ND_IF,        // if
    ND_DO_WHILE,  // do-while
    ND_FOR,       // for
    ND_SWITCH,    // switch
    ND_CASE,      // case
    ND_BREAK,     // break
    ND_CONTINUE,  // continue
    ND_BLOCK,     // block
    ND_CALL,      // function call
    ND_FUNC,      // function definition
    ND_ADDR,      // address of operator ('&')
    ND_DEREF,     // pointer dereference ('*')
    ND_EQ,        // ==
    ND_NE,        // !=
    ND_LE,        // <=
    ND_GE,        // >=
    ND_TY_SIZE,   // size of specific types
    ND_POST_INC,  // post-increment
    ND_POST_DEC,  // post-decrement
    ND_LOG_AND,   // &&
    ND_LOG_OR,    // ||
    ND_LOG_NOT,   // !
    ND_DOT,       // . operator for struct
};

/// AST Node
typedef struct Node {
    int op;     // AST Node Type
    Type *ty;     // C type
    struct Node *lhs;
    struct Node *rhs;
    int val;
    char *name;
    struct Node *expr;

    // 'if' (cond) then 'else' body
    // 'for' (init; cond; inc) body
    // 'while' (cond) body;
    // 'do' body 'while' (cond);
    // function (args) body
    struct Node *cond;
    struct Node *then;
    struct Node *els;
    struct Node *init;
    struct Node *inc;
    struct Node *body;

    // For switch
    Vector *cases;
    int switch_num;

    // For break and continue
    struct Node *target;
    int break_num;
    int continue_num;

    // block statement(compound statement)
    Vector *stmts;

    // function call
    Vector *args;

    // Variable reference
    Var *var;

} Node;

typedef struct Env {
    Map *vars;
    Map *tags;
    struct Env *prev;
} Env;

typedef struct Function {
    Type *ty;   // C type
    char *name;
    Node *node;
    Vector *lvars;
} Function;

typedef struct Program {
    Map *gvars;
    Vector *fns;
} Program;



Node *new_node(int op);
Node *new_node_binop(int op, Node *lhs, Node *rhs);
Node *new_node_num(int val);
int consume(int ty);
Type *ptr_to(Type *base);
Node *expr();
Node *assign();
Node *ternary();
Node *log_or();
Node *log_and();
Node *bit_or();
Node *bit_xor();
Node *bit_and();
Node *equality();
Node *relational();
Node *add();
Node *mul();
Node *unary();
Node *postfix();
Node *term();
Node *param();
Node *declaration();
Var *add_gvar(Type *ty, char *name, char *data, int is_extern);
Program *parse(Vector *v);

/// sema.c
void sema(Program *prog);

/// traverse.c
void traverse(Program *prog);


/// codegen.c
void gen(Node *node);
void gen_lval(Node *node);
void gen_func(Function *fn);
void gen_x86(Program *prog);



/// error functions
void error(char *fmt, ...);
void error_at(char *loc, char *msg);


