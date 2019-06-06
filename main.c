#include "xcc.h"

// ---------------------------------------
char *user_input;
// ---------------------------------------

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

    Vector *tokens = tokenize(argv[1]);
    // printf("%d\n", TK_GE);
    // for (int i = 0; i < tokens_vector->len; i++) {
    //     printf("%d\n",((Token *)tokens_vector->data[i])->val);
    // }
    // return 0;
    // Node *node = expr();
    Node *node = parse(tokens);

    // print assembly
    printf(".intel_syntax noprefix\n");
    printf(".global main\n");
    printf("main:\n");

    gen(node);

    printf(" pop rax\n");
    printf(" ret\n");
    return 0;
}