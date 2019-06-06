#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern char *user_input;

/// container.c
typedef struct {
    void **data;
    int capacity;
    int len;
} Vector;

Vector *new_vector();
void vec_push(Vector *vec, void *elem);
int expect(int line, int expected, int actual);
void runtest();


/// token.c
enum {
    TK_NUM = 256,   // integers
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
    char *input;    // token strings for error message
} Token;

Vector *tokenize(char *p);


/// parse.c
enum {
    ND_NUM = 256,
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
Node *parse(Vector *v);

/// codegen.c
void gen(Node *node);

/// error functions
void error(char *fmt, ...);
void error_at(char *loc, char *msg);