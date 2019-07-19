#include "xcc.h"


void append(Vector *v1, Vector *v2) {
    for (int i = 0; i < v2->len; i++) {
        Token *t = v2->data[i];
        // skip EOF
        // skip new line token
        if (t->ty == '\n' || t->ty == TK_EOF)
            continue;
        vec_push(v1, v2->data[i]);
    }
}

Map *macro;
Vector *preprocess(Vector *tokens) {
    if (!macro)
        macro = new_map();

    Vector *v = new_vector();

    for (int i = 0; i < tokens->len;) {
        Token *t = tokens->data[i];    

        if (t->ty == TK_IDENT) {
            Vector *expansion = map_get(macro, t->name);
            if (expansion)
                append(v, expansion);
            else
                vec_push(v, t);
            i++;
            continue;
        }
        if (t->ty == '\n') {
            i++;
            continue;
        }

        if (t->ty != '#') {
            i++;
            vec_push(v, t);
            continue;
        }

        t = tokens->data[++i];
        if (t->ty != TK_IDENT)
            error_at(t, "identifier expected");

        if (!strcmp(t->name, "include")) {
            t = tokens->data[++i];
            if (t->ty != TK_STRING)
                error_at(t, "string expected");

            char *path = t->name;
            t = tokens->data[++i];
            if (t->ty != '\n')
                error_at(t, "new line expected");

            Vector *nv = tokenize(read_file(path));
            nv = preprocess(nv);
            append(v, nv);
        }
        else if (!strcmp(t->name, "define")) {
            t = tokens->data[++i];
            if (t->ty != TK_IDENT)
                error_at(t, "macro name expected");
            char *name = t->name;

            Vector *expansion = new_vector();
            t = tokens->data[++i];
            while (t->ty != '\n') {
                vec_push(expansion, t);
                t = tokens->data[++i];
            }
            map_put(macro, name, expansion);
        }

        i++;
    }

    // for (int i = 0; i < v->len; i++) {
    //     Token *t = v->data[i];
    //     printf("%d\n", t->ty);
    // }
    return v;
}