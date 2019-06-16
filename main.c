#include "xcc.h"

// ---------------------------------------
char *user_input;
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
    sema(nodes);

    gen_x86(nodes);

    return 0;
}