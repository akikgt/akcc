#include "xcc.h"

/// Error functions
void error(char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");
    exit(1);
}

void error_at(char *loc, char *msg) {
    char *line = loc;
    while (user_input < line && line[-1] != '\n')
        line--;


    char *end = loc;
    while (*end && *end != '\n')
        end++;

    int line_num = 1;
    for (char *p = user_input; p < line; p++)
        if (*p == '\n')
            line_num++;

    int indent = fprintf(stderr, "%d: ", line_num);
    fprintf(stderr, "%.*s\n", (int)(end - line), line); // %.1s is print the first character

    int pos = loc - line + indent;
    fprintf(stderr, "%*s", pos, "");
    fprintf(stderr, "^ %s\n", msg);
    exit(1);
}

/// General
char *format(char *fmt, ...) {
    char buf[2048];
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(buf, sizeof(buf), fmt, ap);
    va_end(ap);
    return strdup(buf);
}

// return specified file content
char *read_file(char *path) {
    // open file
    FILE *fp = fopen(path, "r");
    if (!fp)
        error("cannot open %s: %s", path, strerror(errno));

    // check file length
    if (fseek(fp, 0, SEEK_END) == -1)
        error("%s: fseek: %s", path, strerror(errno));
    size_t size = ftell(fp);
    if (fseek(fp, 0, SEEK_SET) == -1)
        error("%s: fseek: %s", path, strerror(errno));

    // read file content
    char *buf = malloc(size + 2);
    fread(buf, size, 1, fp);

    // make file end with '\n\0'
    if (size == 0 || buf[size - 1] != '\n')
        buf[size++] = '\n';
    buf[size] = '\0';
    fclose(fp);
    return buf;
}


/// Type
int roundup(int x, int align) {
    return (x + align - 1) & ~(align - 1);
}

Type *new_ty(int ty, int size) {
    Type *ret = calloc(1, sizeof(Type));
    ret->ty = ty;
    ret->size = size;
    ret->align = size;
    return ret;
}

Type *void_ty() {
    Type *ty = new_ty(VOID, 1);
    ty->ptr_to = NULL;
    return ty;
}

Type *char_ty() {
    Type *ty = new_ty(CHAR, 1);
    ty->ptr_to = NULL;
    return ty;
}

Type *int_ty() {
    Type *ty = new_ty(INT, 4);
    ty->ptr_to = NULL;
    return ty;
}

Type *enum_ty(int val) {
    Type *ty = new_ty(ENUM, 4);
    ty->val = val;
    ty->ptr_to = NULL;
    return ty;
}

Type *arr_ty(Type *base, int len) {
    Type *ty = new_ty(ARRAY, base->size * len);
    ty->align = base->align;
    ty->array_size = len;
    ty->arr_of = base;
    ty->ptr_to = base;
    return ty;
}

/// Vector
Vector *new_vector() {
    Vector *vec = malloc(sizeof(Vector));
    vec->data = malloc(sizeof(void *) * 16);
    vec->capacity = 16;
    vec->len = 0;
    return vec;
}

void vec_push(Vector *vec, void *elem) {
    if (vec->capacity == vec->len) {
        vec->capacity *= 2;
        vec->data = realloc(vec->data, sizeof(void *) * vec->capacity);
    }
    vec->data[vec->len++] = elem;
}

void *vec_top(Vector *vec) {
    if (!vec->len)
        error("empty vector");
    return vec->data[vec->len - 1];
}

void *vec_pop(Vector *vec) {
    if (!vec->len)
        error("pop from empty vector");
    vec->len--;
    return vec->data[vec->len];
}

static int expect(int line, int expected, int actual) {
    if (expected == actual) 
        return 1;
    fprintf(stderr, "%d: %d expected, but got %d\n",
            line, expected, actual);
    exit(1);
}

void test_vector() {
    printf("Run vector test\n");

    Vector *vec = new_vector();
    expect(__LINE__, 0, vec->len);

    for (long i = 0; i < 100; i++) {
        vec_push(vec, (void *)i);
    }

    expect(__LINE__, 100, vec->len);
    expect(__LINE__, 0, (long)vec->data[0]);
    expect(__LINE__, 50, (long)vec->data[50]);
    expect(__LINE__, 99, (long)vec->data[99]);
    expect(__LINE__, 99, (long)vec_pop(vec));
    expect(__LINE__, 98, (long)vec_top(vec));

    printf("OK\n");
}

/// Map
Map *new_map() {
    Map *map = malloc(sizeof(Map));
    map->keys = new_vector();
    map->vals = new_vector();
    return map;
}

void map_put(Map *map, char *key, void *val) {
    vec_push(map->keys, key);
    vec_push(map->vals, val);
}

void *map_get(Map *map, char *key) {
    for (int i = map->keys->len - 1; i >= 0; i--) {
        if (strcmp(map->keys->data[i], key) == 0)
            return map->vals->data[i];
    }
    return NULL;
}

void test_map() {
    printf("Run map test\n");
    Map *map = new_map();
    expect(__LINE__, 0, (long)map_get(map, "foo"));

    map_put(map, "foo", (void *)2);
    expect(__LINE__, 2, (long)map_get(map, "foo"));

    map_put(map, "bar", (void *)4);
    expect(__LINE__, 4, (long)map_get(map, "bar"));

    map_put(map, "foo", (void *)6);
    expect(__LINE__, 6, (long)map_get(map, "foo"));
}

void runtest() {
    test_vector();
    test_map();
}