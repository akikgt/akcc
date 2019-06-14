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

// test function
void printNodes(Node *node, int depth) {
    if (node == NULL) {
        return;
    }
    printNodes(node->lhs, depth + 1);
    printf("%*s", depth, "");
    printf("%d\n", node->op);
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
    Vector *nodes = parse(tokens);

    // print assembly
    printf(".intel_syntax noprefix\n");
    for (int i = 0; i < nodes->len - 1; i++) {
        gen_func(nodes->data[i]);
    }

    return 0;
}