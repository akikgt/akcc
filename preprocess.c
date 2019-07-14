#include "xcc.h"

void append(Vector *v1, Vector *v2) {
    for (int i = 0; i < v2->len - 1; i++) {
        // v2->len - 1 means skipping TK_EOF
        vec_push(v1, v2->data[i]);
    }
}

Vector *preprocess(Vector *tokens) {
    Vector *v = new_vector();

    for (int i = 0; i < tokens->len;) {
        Token *t = tokens->data[i];    
        if (t->ty != '#') {
            i++;
            vec_push(v, t);
            continue;
        }

        t = tokens->data[++i];
        if (t->ty != TK_IDENT || strcmp(t->name, "include"))
            error_at(t->input, "include expected");

        t = tokens->data[++i];
        if (t->ty != TK_STRING)
            error_at(t->input, "string expected");

        char *path = t->name;
        Vector *nv = tokenize(read_file(path));
        append(v, nv);
        i++;
    }

    // for (int i = 0; i < v->len; i++) {
    //     Token *t = v->data[i];
    //     printf("%d\n", t->ty);
    // }
    return v;
}