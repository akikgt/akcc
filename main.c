#include "akcc.h"

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
        if (argc != 3) {
            fprintf(stderr, "file name expected\n");
            return 1;
        }
        tokens = tokenize(read_file(argv[2]));
    }
    else {
        tokens = tokenize(argv[1]);
    }

    tokens = preprocess(tokens);

    Program *prog = parse(tokens);
    // traverse(prog); // Node test
    sema(prog);
    gen_x86(prog);

    return 0;
}