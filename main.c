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

    Program *prog = parse(tokens);
    sema(prog);

    gen_x86(prog);

    return 0;
}