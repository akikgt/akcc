void alloc4(int **p, int e1, int e2, int e3, int e4) {
    *p = malloc(sizeof(int) * 4);
    *(*p + 0) = e1;
    *(*p + 1) = e2;
    *(*p + 2) = e3;
    *(*p + 3) = e4;
    /// the syntax like *p[2] may not work
    /// p is pointer to pointer
    /// so p[2] means the address of pointer-to-pointer + 16
    /// the space for array is in *p, not in p;
}

void alloc1(int **p, int e1) {
    *p = malloc(sizeof(int));
    *p[0] = e1;
}