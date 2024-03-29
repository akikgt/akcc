#include "akcc.h"

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
    map_put(keywords, "extern", (void *)TK_EXTERN);
    map_put(keywords, "static", (void *)TK_STATIC);
    map_put(keywords, "const", (void *)TK_CONST);
    map_put(keywords, "struct", (void *)TK_STRUCT);
    map_put(keywords, "enum", (void *)TK_ENUM);
    map_put(keywords, "void", (void *)TK_VOID);
    map_put(keywords, "char", (void *)TK_CHAR);
    map_put(keywords, "int", (void *)TK_INT);
    map_put(keywords, "return", (void *)TK_RETURN);
    map_put(keywords, "if", (void *)TK_IF);
    map_put(keywords, "else", (void *)TK_ELSE);
    map_put(keywords, "for", (void *)TK_FOR);
    map_put(keywords, "while", (void *)TK_WHILE);
    map_put(keywords, "do", (void *)TK_DO_WHILE);
    map_put(keywords, "switch", (void *)TK_SWITCH);
    map_put(keywords, "case", (void *)TK_CASE);
    map_put(keywords, "default", (void *)TK_DEFAULT);
    map_put(keywords, "break", (void *)TK_BREAK);
    map_put(keywords, "continue", (void *)TK_CONTINUE);
    map_put(keywords, "sizeof", (void *)TK_SIZEOF);
    map_put(keywords, "typedef", (void *)TK_TYPEDEF);
    return keywords;
}

char *user_input;
Token *make_token(int ty, char *input) {
    Token *token = calloc(1, sizeof(Token));
    token->ty = ty;
    token->input = input;
    token->user_input = user_input;
    return token;
}

Token *add_token(Vector *vec, int ty, char *input) {
    Token *token = make_token(ty, input);
    vec_push(vec, token);
    return token;
}

Vector *tokenize(char *p) {
    Vector *v = new_vector();
    Map *keywords = set_keywords();
    user_input = p; // save original input

    int i = 0;
    while (*p) {
        // for preprocesser. new line token
        if (*p == '\n') {
            add_token(v, '\n', p);
            i++;
            p++;
            continue;
        }

        // skip space characters
        if (isspace(*p)) {
            p++;
            continue;
        }        

        // skip line comment
        if (strncmp(p, "//", 2) == 0) {
            p += 2;
            while (*p != '\n')
                p++;
            continue;
        }
        // skip block comment
        if (strncmp(p, "/*", 2) == 0) {
            char *q = strstr(p + 2, "*/");
            if (!q)
                error_at(make_token(NULL, p), "comment is not closed");
            p = q + 2;
            continue;
        }

        // Ascii character
        if (*p == '\'') {
            p++;

            char val;
            if (*p == '\\') {
                // escape
                p++;
                // new line
                if (*p == 'n')
                    val = 10;
                else if (*p == '0')
                    val = 0;
                else
                    val = *p;
            }
            else {
                val = *p;
            }

            if (p[1] != '\'')
                error_at(make_token(NULL, p), " ' is not closed");

            Token *t = add_token(v, TK_ASCII, p);
            t->val = val;
            i++;
            p += 2;
            continue;
        }

        // String literal
        if (*p == '"') {
            int len = 1;
            while (!(p[len] == '"')) {
                // escape '"'
                if (p[len] == '\\' && p[len + 1] == '"')
                    len += 2;
                else
                    len++;
            }
            len++;

            char *str = strndup(p + 1, len - 2);
            Token *t = add_token(v, TK_STRING, p);
            t->name = str;
            t->len = len - 2;

            i++;
            p += len;
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

        if (strncmp(p, "...", 3) == 0) {
            add_token(v, TK_VARARGS, p);
            i++;
            p += 3;
            continue;
        }
        if (strncmp(p, "->", 2) == 0) {
            add_token(v, TK_ARROW, p);
            i++;
            p += 2;
            continue;
        }
        if (strncmp(p, "<<", 2) == 0) {
            add_token(v, TK_SHL, p);
            i++;
            p += 2;
            continue;
        }
        if (strncmp(p, ">>", 2) == 0) {
            add_token(v, TK_SHR, p);
            i++;
            p += 2;
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
        if (strncmp(p, "+=", 2) == 0) {
            add_token(v, TK_ADD_EQ, p);
            i++;
            p += 2;
            continue;
        }
        if (strncmp(p, "-=", 2) == 0) {
            add_token(v, TK_SUB_EQ, p);
            i++;
            p += 2;
            continue;
        }
        if (strncmp(p, "*=", 2) == 0) {
            add_token(v, TK_MUL_EQ, p);
            i++;
            p += 2;
            continue;
        }
        if (strncmp(p, "/=", 2) == 0) {
            add_token(v, TK_DIV_EQ, p);
            i++;
            p += 2;
            continue;
        }
        if (strncmp(p, "%=", 2) == 0) {
            add_token(v, TK_MOD_EQ, p);
            i++;
            p += 2;
            continue;
        }

        if (strncmp(p, "++", 2) == 0) {
            add_token(v, TK_INC, p);
            i++;
            p += 2;
            continue;
        }
        if (strncmp(p, "--", 2) == 0) {
            add_token(v, TK_DEC, p);
            i++;
            p += 2;
            continue;
        }

        if (strncmp(p, "&&", 2) == 0) {
            add_token(v, TK_LOG_AND, p);
            i++;
            p += 2;
            continue;
        }
        if (strncmp(p, "||", 2) == 0) {
            add_token(v, TK_LOG_OR, p);
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
            || *p == ','        // separator for parameter 
            || *p == '&'        // address or bit-and
            || *p == '!'        // logical not
            || *p == '?' || *p == ':'    // ternary
            || *p == '|' || *p == '^'  || *p == '~'      // bit operations
            || *p == '.'        // dot operator for struct
            || *p == '#'        // macro
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

        error_at(make_token(NULL, p), "Cannot tokenize");
    }

    add_token(v, TK_EOF, p);

    return v;
}
