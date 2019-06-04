#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// token value
enum {
    TK_NUM = 256,   // integers
    TK_EOF,         // EOF
};

// type of token
typedef struct {
    int ty;         // type of token
    int val;        // the value when ty == TK_NUM
    char *input;    // token strings for error message
} Token;

// Token *tokens;

/* 
AST node
*/
enum {
    ND_NUM = 256
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
Node *mul();
Node *term();
void error(char *fmt, ...);
void error_at(char *loc, char *msg);

// ---------------------------------------

char *user_input;
int pos;

Token tokens[100];

// error functions
void error(char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");
    exit(1);
}

void error_at(char *loc, char *msg) {
    int pos = loc - user_input;
    fprintf(stderr, "%s\n", user_input);
    fprintf(stderr, "%*s", pos, "");
    fprintf(stderr, "^ %s\n", msg);
    exit(1);
}

void tokenize(char *p) {
    user_input = p; // save original input

    int i = 0;
    while (*p) {
        // skip space characters
        if (isspace(*p)) {
            p++;
            continue;
        }

        if (*p == '+' || *p == '-' || *p == '*' || *p == '/' || *p == '(' || *p == ')') {
            tokens[i].ty = *p;
            tokens[i].input = p;
            i++;
            p++;
            continue;
        }

        if (isdigit(*p)) {
            tokens[i].ty = TK_NUM;
            tokens[i].input = p;
            tokens[i].val = strtol(p, &p, 10);
            i++;
            continue;
        }

        error_at(p, "Cannot tokenize");
    }

    tokens[i].ty = TK_EOF;
    tokens[i].input = p;
}

// ---------------------------------------

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

int consume(int ty) {
    if (tokens[pos].ty != ty)
        return 0;
    pos++;
    return 1;
}

Node *expr() {
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
    Node *node = term();

    for (;;) {
        if (consume('*'))
            node = new_node('*', node, term());
        else if (consume('/'))
            node = new_node('/', node, term());
        else
            return node;
    }
}

Node *term() {
    if (consume('(')) {
        Node *node = expr();
        if (!consume(')'))
            error_at(tokens[pos].input, "missing closing parentheses");
        return node;
    }

    if (tokens[pos].ty == TK_NUM) 
        return new_node_num(tokens[pos++].val);

    error_at(tokens[pos].input, "non-number or opening parentheses TOken found");
}

// ---------------------------------------

void gen(Node *node) {
    if (node->ty == ND_NUM) {
        printf(" push %d\n", node->val);
        return;
    }

    gen(node->lhs);
    gen(node->rhs);

    printf(" pop rdi\n");
    printf(" pop rax\n");

    switch (node->ty) {
        case '+':
            printf(" add rax, rdi\n");
            break;
        case '-':
            printf(" sub rax, rdi\n");
            break;
        case '*':
            printf(" imul rdi\n");
            break;
        case '/':
            printf(" cqo\n");
            printf(" idiv rdi\n");
    }

    printf(" push rax\n");
}

// ---------------------------------------

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "The number of arguments is invalid \n");
        return 1;
    }

    tokenize(argv[1]);
    Node *node = expr();

    // print assembly
    printf(".intel_syntax noprefix\n");
    printf(".global main\n");
    printf("main:\n");

    gen(node);

    printf(" pop rax\n");
    printf(" ret\n");
    return 0;
}