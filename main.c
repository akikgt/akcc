#include "xcc.h"

// ---------------------------------------
char *user_input;
// ---------------------------------------

int main(int argc, char **argv) {
    if (!(argc == 2 || argc == 3)) {
        fprintf(stderr, "The number of arguments is invalid \n");
        return 1;
    }

    if (!strcmp(argv[1], "-test")) {
        runtest();
        return 0;
    }

    Vector *tokens;
    if (!strcmp(argv[1], "-file")) {
        tokens = tokenize(read_file(argv[2]));
    }
    else {
        tokens = tokenize(argv[1]);
    }

    Program *prog = parse(tokens);
    // traverse(prog); // Node test
    sema(prog);
    gen_x86(prog);

    return 0;
}