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

int is_alnum(char c) {
    return ('a' <= c && c <= 'z') ||
            ('A' <= c && c <= 'Z') ||
            ('0' <= c && c <= '9') ||
            (c == '_');
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
    user_input = p; // save original input

    int i = 0;
    while (*p) {
        // skip space characters
        if (isspace(*p)) {
            p++;
            continue;
        }

        /* C keywords - Reserved words */
        /// Return
        if (strncmp(p, "return", 6) == 0 && !is_alnum(p[6])) {
            add_token(v, TK_RETURN, p);
            i++;
            p += 6;
            continue;
        }

        /// If statement
        if (strncmp(p, "if", 2) == 0 && !is_alnum(p[2])) {
            add_token(v, TK_IF, p);
            i++;
            p += 2;
            continue;
        }

        /// Else statement
        if (strncmp(p, "else", 4) == 0 && !is_alnum(p[4])) {
            add_token(v, TK_ELSE, p);
            i++;
            p += 4;
            continue;
        }

        /// While 
        if (strncmp(p, "while", 5) == 0 && !is_alnum(p[5])) {
            add_token(v, TK_WHILE, p);
            i++;
            p += 5;
            continue;
        }
        /* End - C keywords - Reserved words */

        // Identifier
        if (isalpha(*p) || *p == '_') {
            int len = 1;
            while (isalpha(p[len]) || isdigit(p[len]) ||  p[len] == '_')
                len++;

            Token *t = add_token(v, TK_IDENT, p);
            t->name = strndup(p, len);
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
            *p == '+' || *p == '-' || *p == '*' || *p == '/' 
            || *p == '(' || *p == ')'
            ||  *p == '<' || *p == '>'
            || *p == ';'
            || *p == '='
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

        // if ('a' <= *p && *p <= 'z') {
        //     add_token(v, TK_IDENT, p);
        //     i++;
        //     p++;
        //     continue;
        // }

        error_at(p, "Cannot tokenize");
    }

    add_token(v, TK_EOF, p);

    return v;
}
