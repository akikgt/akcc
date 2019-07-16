// #define _GNU_SOURCE
// #include <ctype.h>
// #include <stdarg.h>
// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <errno.h>

#define va_start __builtin_va_start
#define va_end __builtin_va_end
#define va_arg __builtin_va_arg

typedef struct {
    int gp_offset;
    int fp_offset;
    void *overflow_arg_area;
    void *reg_save_area;
} va_list[1];

typedef int size_t;
// typedef struct _IO_FILE FILE;
typedef void FILE;
// #define FILE struct _IO_FILE
extern FILE *stdin;
extern FILE *stdout;
extern FILE *stderr;
#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2
int ftell(FILE *stream);
int fseek(FILE *fp, int offset, int origin); 
#define EOF (-1)
FILE *fopen(const char *pathname, const char *mode);
int fclose(FILE *stream);

#define NULL ((void *)0)
int printf(const char *format, ...);
int fprintf(FILE *stream, const char *format, ...);
int sprintf(char *str, const char *format, ...);
void exit(int status);
void *malloc(int size);
void *realloc(void *ptr, int size);
void *calloc(size_t n, size_t size);
int strlen(const char *s);
int strcmp(const char *s1, const char *s2);
char *strcpy(char *dest, const char *src);
char *strdup(const char *s);
char *strndup(const char *s, size_t n);
// char *strcat(char *s1, const char *s2);
int vsprintf(char *str, const char *format, va_list ap);
// void *memset(void *s, int c, int n);
// int fgetc(FILE *stream);
int isalpha(int c);
int isdigit(int c);
int isspace(int c);
// void *memcpy(void *dest, const void *src, int n);

extern char *user_input;

// vector
typedef struct {
    void **data;
    int capacity;
    int len;
} Vector;

Vector *new_vector();
void vec_push(Vector *vec, void *elem);
void *vec_pop(Vector *vec);
void *vec_top(Vector *vec);
void runtest();

// map
typedef struct {
    Vector *keys;
    Vector *vals;
} Map;

Map *new_map();
void map_put(Map *map, char *key, void *val);
void *map_get(Map *map, char *key);
void test_map();

/// type
typedef struct Type {
    enum { VOID, CHAR, INT, PTR, ARRAY, STRUCT, ENUM } ty;
    int size;
    int align;

    // Pointer
    struct Type *ptr_to;

    // Array
    struct Type *arr_of;
    int array_size;  // TODO: use size_t

    // Struct
    Map *members;
    int offset;

    // Enum
    int val;
} Type;

/// variable
typedef struct Var {
    Type *ty;
    int offset;
    char *name;
    int is_local;

    // for global variable initial data
    int has_init;
    char *str_data; // string
    int init_val; // numeric values
    Vector *arr_data; // array

    // extern
    int is_extern;
    // static
    int is_static;
    // const
    int is_const;

} Var;

/// util.c
char *format(char *fmt, ...);
char *read_file(char *path);
Type *new_ty(int ty, int size);
Type *void_ty();
Type *char_ty();
Type *int_ty();
Type *enum_ty(int val);
Type *arr_ty(Type *base, int len);
int roundup(int x, int align); 



/// token.c
enum
{
    TK_NUM = 256, // integers
    TK_STRING,    // string literal
    TK_ASCII,     // ascii character
    TK_IDENT,     // identifier
    TK_SIZEOF,    // sizeof
    TK_TYPEDEF,   // typedef
    TK_RETURN,    // return
    TK_IF,        // if
    TK_ELSE,      // else
    TK_WHILE,     // while
    TK_DO_WHILE,  // do while
    TK_FOR,       // for
    TK_SWITCH,    // switch
    TK_CASE,      // case
    TK_BREAK,     // break
    TK_CONTINUE,  // continue
    TK_EXTERN,    // extern
    TK_STATIC,    // static
    TK_CONST,     // const
    TK_STRUCT,    // struct
    TK_ENUM,      // enum
    TK_VOID,      // void
    TK_CHAR,      // char
    TK_INT,       // int
    TK_ARROW,     // ->
    TK_EQ,        // ==
    TK_NE,        // !=
    TK_LE,        // <=
    TK_GE,        // >=
    TK_ADD_EQ,    // +=
    TK_SUB_EQ,    // -=
    TK_MUL_EQ,    // *=
    TK_DIV_EQ,    // /=
    TK_MOD_EQ,    // %=
    TK_INC,       // increment
    TK_DEC,       // decrement
    TK_LOG_AND,   // &&
    TK_LOG_OR,    // ||
    TK_SHL,       // <<
    TK_SHR,       // >>
    TK_VARARGS,   // ...
    TK_EOF,       // EOF
};

typedef struct {
    int ty;         // type of token
    int val;        // the value when ty == TK_NUM
    char *name;     // name of identifier when ty == TK_IDENT

    // string literal
    int len;

    char *input;    // token strings for error message
} Token;

Vector *tokenize(char *p);

/// preprocess.c
Vector *preprocess(Vector *tokens);

/// parse.c
enum
{
    ND_NUM = 256, // integers
    ND_VARDEF,    // variable definition
    ND_IDENT,     // identifier
    ND_SIZEOF,    // sizeof
    ND_RETURN,    // return
    ND_IF,        // if
    ND_DO_WHILE,  // do-while
    ND_FOR,       // for
    ND_SWITCH,    // switch
    ND_CASE,      // case
    ND_BREAK,     // break
    ND_CONTINUE,  // continue
    ND_BLOCK,     // block
    ND_CALL,      // function call
    ND_FUNC,      // function definition
    ND_ADDR,      // address of operator ('&')
    ND_DEREF,     // pointer dereference ('*')
    ND_EQ,        // ==
    ND_NE,        // !=
    ND_LE,        // <=
    ND_GE,        // >=
    ND_TY_SIZE,   // size of specific types
    ND_CAST,      // cast
    ND_POST_INC,  // post-increment
    ND_POST_DEC,  // post-decrement
    ND_LOG_AND,   // &&
    ND_LOG_OR,    // ||
    ND_LOG_NOT,   // !
    ND_DOT,       // . operator for struct
    ND_SHL,       // <<
    ND_SHR,       // >>
    ND_VARARGS,   // ...
};

/// AST Node
typedef struct Node {
    int op;     // AST Node Type
    Type *ty;     // C type
    struct Node *lhs;
    struct Node *rhs;
    int val;
    char *name;
    struct Node *expr;
    int can_be_lval;

    // 'if' (cond) then 'else' body
    // 'for' (init; cond; inc) body
    // 'while' (cond) body;
    // 'do' body 'while' (cond);
    // function (args) body
    struct Node *cond;
    struct Node *then;
    struct Node *els;
    struct Node *init;
    struct Node *inc;
    struct Node *body;

    // For switch
    Vector *cases;
    int switch_num;

    // For break and continue
    struct Node *target;
    int break_num;
    int continue_num;

    // block statement(compound statement)
    Vector *stmts;

    // function call
    Vector *args;

    // Variable reference
    Var *var;

} Node;

typedef struct Env {
    Map *vars;
    Map *tags;
    Map *typedefs;
    Map *enums;
    struct Env *prev;
} Env;

typedef struct Function {
    Type *ty;   // C type
    char *name;
    Node *node;
    Vector *lvars;
} Function;

typedef struct Program {
    Map *gvars;
    Vector *fns;
} Program;



Node *new_node(int op);
Node *new_node_binop(int op, Node *lhs, Node *rhs);
Node *new_node_num(int val);
int consume(int ty);
Type *ptr_to(Type *base);
Node *expr();
Node *assign();
Node *ternary();
Node *log_or();
Node *log_and();
Node *bit_or();
Node *bit_xor();
Node *bit_and();
Node *equality();
Node *relational();
Node *shift();
Node *add();
Node *mul();
Node *unary();
Node *postfix();
Node *term();
Node *param();
Node *declaration();
Var *add_gvar(Type *ty, char *name, char *data, int is_extern);
Program *parse(Vector *v);

/// sema.c
void sema(Program *prog);

/// traverse.c
void traverse(Program *prog);


/// codegen.c
void gen(Node *node);
void gen_lval(Node *node);
void gen_func(Function *fn);
void gen_x86(Program *prog);



/// error functions
void error(char *fmt, ...);
void error_at(char *loc, char *msg);



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
    map_put(keywords, "break", (void *)TK_BREAK);
    map_put(keywords, "continue", (void *)TK_CONTINUE);
    map_put(keywords, "sizeof", (void *)TK_SIZEOF);
    map_put(keywords, "typedef", (void *)TK_TYPEDEF);
    return keywords;
}

Token *add_token(Vector *vec, int ty, char *input) {
    Token *token = calloc(1, sizeof(Token));
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
                error_at(p, "comment is not closed");
            p = q + 2;
            continue;
        }

        // Ascii character
        if (*p == '\'') {
            p++;
            if (*p == '\\') {
                // TODO:escape char
                p++;
            }

            if (p[1] != '\'')
                error_at(p, " ' is not closed");

            Token *t = add_token(v, TK_ASCII, p);
            t->val = *p;
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

        error_at(p, "Cannot tokenize");
    }

    add_token(v, TK_EOF, p);

    return v;
}
