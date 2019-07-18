// // #define _GNU_SOURCE
// // #include <ctype.h>
// // #include <stdarg.h>
// // #include <stdio.h>
// // #include <stdlib.h>
// // #include <string.h>
// // #include <errno.h>



// #define va_start __builtin_va_start
// #define va_end __builtin_va_end
// #define va_arg __builtin_va_arg

// typedef struct {
//     int gp_offset;
//     int fp_offset;
//     void *overflow_arg_area;
//     void *reg_save_area;
// } va_list[1];

// typedef int size_t;
// // typedef struct _IO_FILE FILE;
// typedef void FILE;
// // #define FILE struct _IO_FILE
// extern FILE *stdin;
// extern FILE *stdout;
// extern FILE *stderr;
// #define SEEK_SET 0
// #define SEEK_CUR 1
// #define SEEK_END 2
// int ftell(FILE *stream);
// int fseek(FILE *fp, int offset, int origin); 
// #define EOF (-1)
// FILE *fopen(const char *pathname, const char *mode);
// int fclose(FILE *stream);

// #define NULL ((void *)0)
// int printf(const char *format, ...);
// int fprintf(FILE *stream, const char *format, ...);
// int sprintf(char *str, const char *format, ...);
// void exit(int status);
// void *malloc(int size);
// void *realloc(void *ptr, int size);
// void *calloc(size_t n, size_t size);
// int strlen(const char *s);
// int strcmp(const char *s1, const char *s2);
// char *strcpy(char *dest, const char *src);
// char *strdup(const char *s);
// char *strndup(const char *s, size_t n);
// int vsprintf(char *str, const char *format, va_list ap);
// // void *memset(void *s, int c, int n);
// // int fgetc(FILE *stream);
// int isalpha(int c);
// int isdigit(int c);
// int isspace(int c);
// // void *memcpy(void *dest, const void *src, int n);

// extern char *user_input;

// // vector
// typedef struct {
//     void **data;
//     int capacity;
//     int len;
// } Vector;

// Vector *new_vector();
// void vec_push(Vector *vec, void *elem);
// void *vec_pop(Vector *vec);
// void *vec_top(Vector *vec);
// void runtest();

// // map
// typedef struct {
//     Vector *keys;
//     Vector *vals;
// } Map;

// Map *new_map();
// void map_put(Map *map, char *key, void *val);
// void *map_get(Map *map, char *key);
// void test_map();

// /// type
// typedef struct Type {
//     enum { VOID, CHAR, INT, PTR, ARRAY, STRUCT, ENUM } ty;
//     int size;
//     int align;

//     // Pointer
//     struct Type *ptr_to;

//     // Array
//     struct Type *arr_of;
//     int array_size;  // TODO: use size_t

//     // Struct
//     Map *members;
//     int offset;

//     // Enum
//     int val;
// } Type;

// /// variable
// typedef struct Var {
//     Type *ty;
//     int offset;
//     char *name;
//     int is_local;

//     // for global variable initial data
//     int has_init;
//     char *str_data; // string
//     int init_val; // numeric values
//     Vector *arr_data; // array

//     // extern
//     int is_extern;
//     // static
//     int is_static;
//     // const
//     int is_const;

// } Var;

// /// util.c
// char *format(char *fmt, ...);
// char *read_file(char *path);
// Type *new_ty(int ty, int size);
// Type *void_ty();
// Type *char_ty();
// Type *int_ty();
// Type *enum_ty(int val);
// Type *arr_ty(Type *base, int len);
// int roundup(int x, int align); 



// /// token.c
// enum
// {
//     TK_NUM = 256, // integers
//     TK_STRING,    // string literal
//     TK_ASCII,     // ascii character
//     TK_IDENT,     // identifier
//     TK_SIZEOF,    // sizeof
//     TK_TYPEDEF,   // typedef
//     TK_RETURN,    // return
//     TK_IF,        // if
//     TK_ELSE,      // else
//     TK_WHILE,     // while
//     TK_DO_WHILE,  // do while
//     TK_FOR,       // for
//     TK_SWITCH,    // switch
//     TK_CASE,      // case
//     TK_BREAK,     // break
//     TK_CONTINUE,  // continue
//     TK_EXTERN,    // extern
//     TK_STATIC,    // static
//     TK_CONST,     // const
//     TK_STRUCT,    // struct
//     TK_ENUM,      // enum
//     TK_VOID,      // void
//     TK_CHAR,      // char
//     TK_INT,       // int
//     TK_ARROW,     // ->
//     TK_EQ,        // ==
//     TK_NE,        // !=
//     TK_LE,        // <=
//     TK_GE,        // >=
//     TK_ADD_EQ,    // +=
//     TK_SUB_EQ,    // -=
//     TK_MUL_EQ,    // *=
//     TK_DIV_EQ,    // /=
//     TK_MOD_EQ,    // %=
//     TK_INC,       // increment
//     TK_DEC,       // decrement
//     TK_LOG_AND,   // &&
//     TK_LOG_OR,    // ||
//     TK_SHL,       // <<
//     TK_SHR,       // >>
//     TK_VARARGS,   // ...
//     TK_EOF,       // EOF
// };

// typedef struct {
//     int ty;         // type of token
//     int val;        // the value when ty == TK_NUM
//     char *name;     // name of identifier when ty == TK_IDENT

//     // string literal
//     int len;

//     char *input;    // token strings for error message
// } Token;

// Vector *tokenize(char *p);

// /// preprocess.c
// Vector *preprocess(Vector *tokens);

// /// parse.c
// enum
// {
//     ND_NUM = 256, // integers
//     ND_VARDEF,    // variable definition
//     ND_IDENT,     // identifier
//     ND_SIZEOF,    // sizeof
//     ND_RETURN,    // return
//     ND_IF,        // if
//     ND_DO_WHILE,  // do-while
//     ND_FOR,       // for
//     ND_SWITCH,    // switch
//     ND_CASE,      // case
//     ND_BREAK,     // break
//     ND_CONTINUE,  // continue
//     ND_BLOCK,     // block
//     ND_CALL,      // function call
//     ND_FUNC,      // function definition
//     ND_ADDR,      // address of operator ('&')
//     ND_DEREF,     // pointer dereference ('*')
//     ND_EQ,        // ==
//     ND_NE,        // !=
//     ND_LE,        // <=
//     ND_GE,        // >=
//     ND_TY_SIZE,   // size of specific types
//     ND_CAST,      // cast
//     ND_POST_INC,  // post-increment
//     ND_POST_DEC,  // post-decrement
//     ND_LOG_AND,   // &&
//     ND_LOG_OR,    // ||
//     ND_LOG_NOT,   // !
//     ND_DOT,       // . operator for struct
//     ND_SHL,       // <<
//     ND_SHR,       // >>
//     ND_VARARGS,   // ...
// };

// /// AST Node
// typedef struct Node {
//     int op;     // AST Node Type
//     Type *ty;     // C type
//     struct Node *lhs;
//     struct Node *rhs;
//     int val;
//     char *name;
//     struct Node *expr;
//     int can_be_lval;

//     // 'if' (cond) then 'else' body
//     // 'for' (init; cond; inc) body
//     // 'while' (cond) body;
//     // 'do' body 'while' (cond);
//     // function (args) body
//     struct Node *cond;
//     struct Node *then;
//     struct Node *els;
//     struct Node *init;
//     struct Node *inc;
//     struct Node *body;

//     // For switch
//     Vector *cases;
//     int switch_num;

//     // For break and continue
//     struct Node *target;
//     int break_num;
//     int continue_num;

//     // block statement(compound statement)
//     Vector *stmts;

//     // function call
//     Vector *args;

//     // Variable reference
//     Var *var;

// } Node;

// typedef struct Env {
//     Map *vars;
//     Map *tags;
//     Map *typedefs;
//     Map *enums;
//     struct Env *prev;
// } Env;

// typedef struct Function {
//     Type *ty;   // C type
//     char *name;
//     Node *node;
//     Vector *lvars;
// } Function;

// typedef struct Program {
//     Map *gvars;
//     Vector *fns;
// } Program;



// Node *new_node(int op);
// Node *new_node_binop(int op, Node *lhs, Node *rhs);
// Node *new_node_num(int val);
// int consume(int ty);
// Type *ptr_to(Type *base);
// Node *expr();
// Node *assign();
// Node *ternary();
// Node *log_or();
// Node *log_and();
// Node *bit_or();
// Node *bit_xor();
// Node *bit_and();
// Node *equality();
// Node *relational();
// Node *shift();
// Node *add();
// Node *mul();
// Node *unary();
// Node *postfix();
// Node *term();
// Node *param();
// Node *declaration();
// Var *add_gvar(Type *ty, char *name, char *data, int is_extern);
// Program *parse(Vector *v);

// /// sema.c
// void sema(Program *prog);

// /// traverse.c
// void traverse(Program *prog);


// /// codegen.c
// void gen(Node *node);
// void gen_lval(Node *node);
// void gen_func(Function *fn);
// void gen_x86(Program *prog);



// /// error functions
// void error(char *fmt, ...);
// void error_at(char *loc, char *msg);



// ///////////
// /// Error functions
// void error(char *fmt, ...) {
//     va_list ap;
//     va_start(ap, fmt);
//     vfprintf(stderr, fmt, ap);
//     fprintf(stderr, "\n");
//     exit(1);
// }

// void error_at(char *loc, char *msg) {
//     char *line = loc;
//     while (user_input < line && line[-1] != '\n')
//         line--;


//     char *end = loc;
//     while (*end && *end != '\n')
//         end++;

//     int line_num = 1;
//     for (char *p = user_input; p < line; p++)
//         if (*p == '\n')
//             line_num++;

//     int indent = fprintf(stderr, "%d: ", line_num);
//     fprintf(stderr, "%.*s\n", (int)(end - line), line); // %.1s is print the first character

//     int pos = loc - line + indent;
//     fprintf(stderr, "%*s", pos, "");
//     fprintf(stderr, "^ %s\n", msg);
//     exit(1);
// }

// /// General
// char *format(char *fmt, ...) {
//     char buf[2048];
//     va_list ap;
//     va_start(ap, fmt);
//     vsnprintf(buf, sizeof(buf), fmt, ap);
//     va_end(ap);
//     return strdup(buf);
// }

// // return specified file content
// char *read_file(char *path) {
//     int errno = 1;
//     // open file
//     FILE *fp = fopen(path, "r");
//     if (!fp) {
//         fprintf(stderr, "cannot open %s\n", path);
//         exit(1);
//     }

//     // check file length
//     if (fseek(fp, 0, SEEK_END) == -1) {
//         fprintf(stderr, "%s: fseek error\n", path);
//         exit(1);
//     }
//     size_t size = ftell(fp);
//     if (fseek(fp, 0, SEEK_SET) == -1) {
//         fprintf(stderr, "%s: fseek error\n", path);
//         exit(1);
//     }

//     // read file content
//     char *buf = malloc(size + 2);
//     fread(buf, size, 1, fp);

//     // make file end with '\n\0'
//     if (size == 0 || buf[size - 1] != '\n')
//         buf[size++] = '\n';
//     buf[size] = '\0';
//     fclose(fp);
//     return buf;
// }


// /// Type
// int roundup(int x, int align) {
//     return (x + align - 1) & ~(align - 1);
// }

// Type *new_ty(int ty, int size) {
//     Type *ret = calloc(1, sizeof(Type));
//     ret->ty = ty;
//     ret->size = size;
//     ret->align = size;
//     return ret;
// }

// Type *void_ty() {
//     Type *ty = new_ty(VOID, 1);
//     ty->ptr_to = NULL;
//     return ty;
// }

// Type *char_ty() {
//     Type *ty = new_ty(CHAR, 1);
//     ty->ptr_to = NULL;
//     return ty;
// }

// Type *int_ty() {
//     Type *ty = new_ty(INT, 4);
//     ty->ptr_to = NULL;
//     return ty;
// }

// Type *enum_ty(int val) {
//     Type *ty = new_ty(ENUM, 4);
//     ty->val = val;
//     ty->ptr_to = NULL;
//     return ty;
// }

// Type *arr_ty(Type *base, int len) {
//     Type *ty = new_ty(ARRAY, base->size * len);
//     ty->align = base->align;
//     ty->array_size = len;
//     ty->arr_of = base;
//     ty->ptr_to = base;
//     return ty;
// }

// /// Vector
// Vector *new_vector() {
//     Vector *vec = malloc(sizeof(Vector));
//     vec->data = malloc(8 * 16);
//     vec->capacity = 16;
//     vec->len = 0;
//     return vec;
// }

// void vec_push(Vector *vec, void *elem) {
//     if (vec->capacity == vec->len) {
//         vec->capacity *= 2;
//         vec->data = realloc(vec->data, sizeof(void *) * vec->capacity);
//     }
//     vec->data[vec->len++] = elem;
// }

// void *vec_top(Vector *vec) {
//     if (!vec->len)
//         error("empty vector");
//     return vec->data[vec->len - 1];
// }

// void *vec_pop(Vector *vec) {
//     if (!vec->len)
//         error("pop from empty vector");
//     vec->len--;
//     return vec->data[vec->len];
// }

// static int expect(int line, int expected, int actual) {
//     if (expected == actual) 
//         return 1;
//     fprintf(stderr, "%d: %d expected, but got %d\n",
//             line, expected, actual);
//     exit(1);
// }

// void test_vector() {
//     printf("Run vector test\n");

//     Vector *vec = new_vector();

//     for (int i = 0; i < 100; i++) {
//         vec_push(vec, (void *)i);
//     }

//     printf("OK\n");
// }

// /// Map
// Map *new_map() {
//     Map *map = calloc(1, sizeof(Map));
//     map->keys = new_vector();
//     map->vals = new_vector();
//     return map;
// }

// void map_put(Map *map, char *key, void *val) {
//     vec_push(map->keys, key);
//     vec_push(map->vals, val);
// }

// void *map_get(Map *map, char *key) {
//     for (int i = map->keys->len - 1; i >= 0; i--) {
//         if (strcmp(map->keys->data[i], key) == 0)
//             return map->vals->data[i];
//     }
//     return NULL;
// }

// void test_map() {
//     printf("Run map test\n");
//     Map *map = new_map();
// }

// void runtest() {
//     test_vector();
//     test_map();
// }