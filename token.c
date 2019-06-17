#include "xcc.h"

// #define _GNU_SOURCEを書かないと使えない？ strdup is not included in standard C
// char *strndup(const char *s, size_t n) {
//     char *p = memchr(s, '\0', n);
//     if (p != NULL)
//         n = p - s;
//     p = malloc(n + 1);
//     if (p != NULL) {
//         memcpy(p, s, n);
//         p[n] = '\0';
//     }
//     return p;
// }
// static Map *keywords;

Map *set_keywords() {
    Map *keywords = new_map();
    map_put(keywords, "int", (void *)TK_INT);
    map_put(keywords, "char", (void *)TK_CHAR);
    map_put(keywords, "return", (void *)TK_RETURN);
    map_put(keywords, "if", (void *)TK_IF);
    map_put(keywords, "else", (void *)TK_ELSE);
    map_put(keywords, "for", (void *)TK_FOR);
    map_put(keywords, "while", (void *)TK_WHILE);
    map_put(keywords, "sizeof", (void *)TK_SIZEOF);
    return keywords;
}

Token *add_token(Vector *vec, int ty, char *input) {
    Token *token = malloc(sizeof(Token));
    token->ty = ty;
    token->input = input;
    vec_push(vec, token);
    return token;
}

Vector *tokenize(char *p) {
    Vector *v = new_vector();
    Map *keywords = set_keywords();
    user_input = p; // save original input

    int i = 0;
    while (*p) {
        // skip space characters
        if (isspace(*p)) {
            p++;
            continue;
        }

        if (isalpha(*p) || *p == '_') {
            int len = 1;
            while (isalpha(p[len]) || isdigit(p[len]) ||  p[len] == '_')
                len++;

            char *name = strndup(p, len);
            if (map_get(keywords, name) != NULL) {
                // Keywords
                add_token(v, map_get(keywords, name), p);
            }
            else {
                // Identifier
                Token *t = add_token(v, TK_IDENT, p);
                t->name = name;
            }
            i++;
            p += len;
            continue;
        }

        if (strncmp(p, "==", 2) == 0) {
            add_token(v, TK_EQ, p);
            i++;
            p += 2;
            continue;
        }
        if (strncmp(p, "!=", 2) == 0) {
            add_token(v, TK_NE, p);
            i++;
            p += 2;
            continue;
        }
        if (strncmp(p, "<=", 2) == 0) {
            add_token(v, TK_LE, p);
            i++;
            p += 2;
            continue;
        }
        if (strncmp(p, ">=", 2) == 0) {
            add_token(v, TK_GE, p);
            i++;
            p += 2;
            continue;
        }

        if (
            *p == '+' || *p == '-' || *p == '*' || *p == '/' || *p == '%'
            || *p == '(' || *p == ')'
            ||  *p == '<' || *p == '>' // inequality
            || *p == ';'    // end of stmt
            || *p == '='    // assign
            || *p == '{' || *p == '}'   // block
            || *p == '[' || *p == ']'   // array
            || *p == ','
            || *p == '&'        // address
            ) {
            add_token(v, *p, p);
            i++;
            p++;
            continue;
        }

        if (isdigit(*p)) {
            Token *t = add_token(v, TK_NUM, p);
            t->val = strtol(p, &p, 10);
            i++;
            continue;
        }

        error_at(p, "Cannot tokenize");
    }

    add_token(v, TK_EOF, p);

    return v;
}
