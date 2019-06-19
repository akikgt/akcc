#define _GNU_SOURCE
#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

extern char *user_input;

/// type
typedef struct Type {
    enum { INT, CHAR, PTR, ARRAY } ty;
    int size;

    // Pointer
    struct Type *ptr_to;

    // Array
    struct Type *arr_of;
    int array_size;  // TODO: use size_t
} Type;

/// util.c
char *format(char *fmt, ...);
char *read_file(char *path);
Type *new_ty(int ty, int size);
Type *int_ty();
Type *char_ty();
Type *arr_ty(Type *base, int len);

// vector
typedef struct {
    void **data;
    int capacity;
    int len;
} Vector;

Vector *new_vector();
void vec_push(Vector *vec, void *elem);
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

// extern Map* map;


/// token.c
enum
{
    TK_NUM = 256, // integers
    TK_STRING,    // string literal
    TK_IDENT,     // identifier
    TK_SIZEOF,    // sizeof
    TK_RETURN,    // return
    TK_IF,        // if
    TK_ELSE,      // else
    TK_WHILE,     // while
    TK_FOR,       // for
    TK_INT,       // int
    TK_CHAR,      // char
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
    ND_WHILE,     // while
    ND_FOR,       // for
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
    ND_POST_INC   // post-increment
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
    // function (args) body
    struct Node *cond;
    struct Node *then;
    struct Node *els;
    struct Node *init;
    struct Node *inc;
    struct Node *body;

    // block statement(compound statement)
    Vector *stmts;

    // function call
    Vector *args;

} Node;

typedef struct Function {
    // int ty;
    Type *ty;   // C type
    char *name;
    Node *node;
    Map *vars;
} Function;


/// variable
typedef struct Var {
    Type *ty;
    int offset;
    char *name;

    // for global variable initial data
    char *data;
} Var;

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
Node *equality();
Node *relational();
Node *add();
Node *mul();
Node *unary();
Node *term();
Node *param();
Node *declaration();
void add_gvar(Type *ty, char *name, char *data);
Program *parse(Vector *v);

/// sema.c
void sema(Program *prog);

/// codegen.c
void gen(Node *node);
void gen_lval(Node *node);
void gen_func(Function *fn);
void gen_x86(Program *prog);
int roundup(int x, int align); 



/// error functions
void error(char *fmt, ...);
void error_at(char *loc, char *msg);


