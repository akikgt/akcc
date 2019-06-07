#define _GNU_SOURCE
#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern char *user_input;

/// container.c
// vector
typedef struct {
    void **data;
    int capacity;
    int len;
} Vector;

Vector *new_vector();
void vec_push(Vector *vec, void *elem);
int expect(int line, int expected, int actual);
void runtest();

// map
typedef struct {
    Vector *keys;
    Vector *vals;
} Map;
void test_map();


/// token.c
enum {
    TK_NUM = 256,   // integers
    TK_IDENT,       // identifier
    TK_RETURN,       // return
    TK_EQ,          // ==
    TK_NE,          // !=
    TK_LE,          // <=
    TK_GE,          // >=
    TK_EOF,         // EOF
};

// type of token
typedef struct {
    int ty;         // type of token
    int val;        // the value when ty == TK_NUM
    char *name;     // name of identifier when ty == TK_IDENT
    char *input;    // token strings for error message
} Token;

Vector *tokenize(char *p);


/// parse.c
enum {
    ND_NUM = 256,
    ND_IDENT,
    ND_RETURN,       // return
    ND_EQ,          // ==
    ND_NE,          // !=
    ND_LE,          // <=
    ND_GE,          // >=
};

typedef struct Node {
    int ty;
    struct Node *lhs;
    struct Node *rhs;
    int val;
    char *name;
} Node;


Node *new_node(int ty, Node *lhs, Node *rhs);
Node *new_node_num(int val);
int consume(int ty);
Node *expr();
Node *equality();
Node *relational();
Node *add();
Node *mul();
Node *unary();
Node *term();
Vector *parse(Vector *v);

/// codegen.c
void gen(Node *node);

/// error functions
void error(char *fmt, ...);
void error_at(char *loc, char *msg);