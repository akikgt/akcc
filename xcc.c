#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ---------------------------------------

// vector

typedef struct {
    void **data;
    int capacity;
    int len;
} Vector;

Vector *new_vector() {
    Vector *vec = malloc(sizeof(Vector));
    vec->data = malloc(sizeof(void *) * 16);
    vec->capacity = 16;
    vec->len = 0;
    return vec;
}

void vec_push(Vector *vec, void *elem) {
    if (vec->capacity == vec->len) {
        vec->capacity *= 2;
        vec->data = realloc(vec->data, sizeof(void *) * vec->capacity);
    }
    vec->data[vec->len++] = elem;
}

int expect(int line, int expected, int actual) {
    if (expected == actual) 
        return 1;
    fprintf(stderr, "%d: %d expected, but got %d\n",
            line, expected, actual);
    exit(1);
}

void runtest() {
    Vector *vec = new_vector();
    expect(__LINE__, 0, vec->len);

    for (long i = 0; i < 100; i++) {
        vec_push(vec, (void *)i);
    }

    expect(__LINE__, 100, vec->len);
    expect(__LINE__, 0, (long)vec->data[0]);
    expect(__LINE__, 50, (long)vec->data[50]);
    expect(__LINE__, 99, (long)vec->data[99]);

    printf("OK\n");
}

// ---------------------------------------

// token value
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

// Token *tokens;

/* 
AST node
*/
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
void error(char *fmt, ...);
void error_at(char *loc, char *msg);

// ---------------------------------------

char *user_input;
int pos;

// Vector *tokens;
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




// ---------------------------------------
// token
void add_token(Vector *vec, int ty, int val) {
    Token *token = malloc(sizeof(Token));
    token->ty = ty;
    // token->input = input;
    token->val = val;
    vec_push(vec, (void *)token);
}

Vector *tokenize(char *p) {
    Vector *v = new_vector();
    user_input = p; // save original input

    int i = 0;
    while (*p) {
        // skip space characters
        if (isspace(*p)) {
            p++;
            continue;
        }

        if (strncmp(p, "==", 2) == 0) {
            // tokens[i].ty = TK_EQ;
            // tokens[i].input = p;
            add_token(v, TK_EQ, 0);
            i++;
            p += 2;
            continue;
        }
        if (strncmp(p, "!=", 2) == 0) {
            // tokens[i].ty = TK_NE;
            // tokens[i].input = p;
            add_token(v, TK_NE, 0);
            i++;
            p += 2;
            continue;
        }
        if (strncmp(p, "<=", 2) == 0) {
            // tokens[i].ty = TK_LE;
            // tokens[i].input = p;
            add_token(v, TK_LE, 0);
            i++;
            p += 2;
            continue;
        }
        if (strncmp(p, ">=", 2) == 0) {
            // tokens[i].ty = TK_GE;
            // tokens[i].input = p;
            add_token(v, TK_GE, 0);
            i++;
            p += 2;
            continue;
        }

        if (
            *p == '+' || *p == '-' || *p == '*' || *p == '/' 
            || *p == '(' || *p == ')'
            ||  *p == '<' || *p == '>'
            ) {
            // tokens[i].ty = *p;
            // tokens[i].input = p;
            add_token(v, *p, 0);
            i++;
            p++;
            continue;
        }

        if (isdigit(*p)) {
            // tokens[i].ty = TK_NUM;
            // tokens[i].input = p;
            // tokens[i].val = strtol(p, &p, 10);
            add_token(v, TK_NUM, strtol(p, &p, 10));
            i++;
            continue;
        }

        error_at(p, "Cannot tokenize");
    }

    // tokens[i].ty = TK_EOF;
    // tokens[i].input = p;
    add_token(v, TK_EOF, 0);

    return v;
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
    Node *node = equality();
    return node;
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
    if (consume('(')) {
        Node *node = expr();
        if (!consume(')'))
            error_at(tokens[pos].input, "missing closing parentheses");
        return node;
    }

    if (tokens[pos].ty == TK_NUM) 
        return new_node_num(tokens[pos++].val);

    error_at(tokens[pos].input, "non-number or opening parentheses Token found");
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
            break;
        case ND_EQ:
            printf(" cmp rax, rdi\n");
            printf(" sete al\n");
            printf(" movzb rax, al\n");
            break;
        case ND_NE:
            printf(" cmp rax, rdi\n");
            printf(" setne al\n");
            printf(" movzb rax, al\n");
            break;
        case ND_LE:
            printf(" cmp rax, rdi\n");
            printf(" setle al\n");
            printf(" movzb rax, al\n");
            break;
        case '<':
            printf(" cmp rax, rdi\n");
            printf(" setl al\n");
            printf(" movzb rax, al\n");
            break;
    }

    printf(" push rax\n");
}

// ---------------------------------------

// char *nodeNames[] = {
//     [256] = "NUMBER",
//     [257] = "EQ",
//     [258] = "NE",
//     [259] = "LE",
//     [260] = "GE",
// };

void printNodes(Node *node, int depth) {
    if (node == NULL) {
        return;
    }
    printNodes(node->lhs, depth + 1);
    printf("%*s", depth, "");
    // printf("%s\n", nodeNames[node->ty]);
    printf("%d\n", node->ty);
    printNodes(node->rhs, depth + 1);
}
// ---------------------------------------

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "The number of arguments is invalid \n");
        return 1;
    }

    if (!strcmp(argv[1], "-test")) {
        runtest();
        return 0;
    }

    Vector *tokens_vector = tokenize(argv[1]);
    // printf("%d\n", TK_GE);
    for (int i = 0; i < 10; i++) {
        printf("%d\n",((Token *)tokens_vector->data[i])->ty);
    }
    return 0;
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