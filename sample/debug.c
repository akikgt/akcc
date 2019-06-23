extern void *stderr;
int one() { return 1; }
int two() { return 2; }
int plus(int x, int y) { return x + y; }
int mul(int x, int y) { return x * y; }
int add(int a, int b, int c, int d, int e, int f) { return a+b+c+d+e+f; }
int add3(int a[][2]) { return a[0][0] + a[1][0]; }
int add4(int a[2][2]) { return a[0][0] + a[1][0]; }
int var1;
int var2[5];
extern int global_arr[1];
int main() {
  do { int e1 = (0); int e2 = (0); if (e1 == e2) { fprintf(stderr, "%s => %d\n", "0", e2); } else { fprintf(stderr, "line %d: %s: %d expected, but got %d\n", 42, "0", e1, e2); exit(1); } } while (0);
  do { int e1 = (0); int e2 = (4 == 5); if (e1 == e2) { fprintf(stderr, "%s => %d\n", "4 == 5", e2); } else { fprintf(stderr, "line %d: %s: %d expected, but got %d\n", 85, "4 == 5", e1, e2); exit(1); } } while (0);
  do { int e1 = (1); int e2 = (5 == 5); if (e1 == e2) { fprintf(stderr, "%s => %d\n", "5 == 5", e2); } else { fprintf(stderr, "line %d: %s: %d expected, but got %d\n", 86, "5 == 5", e1, e2); exit(1); } } while (0);
  do { int e1 = (4); int e2 = (19 % 5); if (e1 == e2) { fprintf(stderr, "%s => %d\n", "19 % 5", e2); } else { fprintf(stderr, "line %d: %s: %d expected, but got %d\n", 101, "19 % 5", e1, e2); exit(1); } } while (0);
  do { int e1 = (0); int e2 = (9 % 3); if (e1 == e2) { fprintf(stderr, "%s => %d\n", "9 % 3", e2); } else { fprintf(stderr, "line %d: %s: %d expected, but got %d\n", 102, "9 % 3", e1, e2); exit(1); } } while (0);
  do { int e1 = (0-3); int e2 = (-3); if (e1 == e2) { fprintf(stderr, "%s => %d\n", "-3", e2); } else { fprintf(stderr, "line %d: %s: %d expected, but got %d\n", 104, "-3", e1, e2); exit(1); } } while (0);
  do { int e1 = (0); int e2 = (!1); if (e1 == e2) { fprintf(stderr, "%s => %d\n", "!1", e2); } else { fprintf(stderr, "line %d: %s: %d expected, but got %d\n", 106, "!1", e1, e2); exit(1); } } while (0);
  do { int e1 = (1); int e2 = (!0); if (e1 == e2) { fprintf(stderr, "%s => %d\n", "!0", e2); } else { fprintf(stderr, "line %d: %s: %d expected, but got %d\n", 107, "!0", e1, e2); exit(1); } } while (0);
  do { int e1 = (-1); int e2 = (~0); if (e1 == e2) { fprintf(stderr, "%s => %d\n", "~0", e2); } else { fprintf(stderr, "line %d: %s: %d expected, but got %d\n", 109, "~0", e1, e2); exit(1); } } while (0);
  do { int e1 = (-4); int e2 = (~3); if (e1 == e2) { fprintf(stderr, "%s => %d\n", "~3", e2); } else { fprintf(stderr, "line %d: %s: %d expected, but got %d\n", 110, "~3", e1, e2); exit(1); } } while (0);
  do { int e1 = (3); int e2 = (({ int i = 3; i++; })); if (e1 == e2) { fprintf(stderr, "%s => %d\n", "({ int i = 3; i++; })", e2); } else { fprintf(stderr, "line %d: %s: %d expected, but got %d\n", 112, "({ int i = 3; i++; })", e1, e2); exit(1); } } while (0);
  do { int e1 = (4); int e2 = (({ int i = 3; ++i; })); if (e1 == e2) { fprintf(stderr, "%s => %d\n", "({ int i = 3; ++i; })", e2); } else { fprintf(stderr, "line %d: %s: %d expected, but got %d\n", 113, "({ int i = 3; ++i; })", e1, e2); exit(1); } } while (0);
  do { int e1 = (3); int e2 = (({ int j = 3; j--; })); if (e1 == e2) { fprintf(stderr, "%s => %d\n", "({ int j = 3; j--; })", e2); } else { fprintf(stderr, "line %d: %s: %d expected, but got %d\n", 114, "({ int j = 3; j--; })", e1, e2); exit(1); } } while (0);
  do { int e1 = (2); int e2 = (({ int i = 3; --i; })); if (e1 == e2) { fprintf(stderr, "%s => %d\n", "({ int i = 3; --i; })", e2); } else { fprintf(stderr, "line %d: %s: %d expected, but got %d\n", 115, "({ int i = 3; --i; })", e1, e2); exit(1); } } while (0);
  do { int e1 = (5); int e2 = (0 ? 3 : 5); if (e1 == e2) { fprintf(stderr, "%s => %d\n", "0 ? 3 : 5", e2); } else { fprintf(stderr, "line %d: %s: %d expected, but got %d\n", 117, "0 ? 3 : 5", e1, e2); exit(1); } } while (0);
  do { int e1 = (3); int e2 = (1 ? 3 : 5); if (e1 == e2) { fprintf(stderr, "%s => %d\n", "1 ? 3 : 5", e2); } else { fprintf(stderr, "line %d: %s: %d expected, but got %d\n", 118, "1 ? 3 : 5", e1, e2); exit(1); } } while (0);
  do { int e1 = (11); int e2 = (9 | 2); if (e1 == e2) { fprintf(stderr, "%s => %d\n", "9 | 2", e2); } else { fprintf(stderr, "line %d: %s: %d expected, but got %d\n", 122, "9 | 2", e1, e2); exit(1); } } while (0);
  do { int e1 = (11); int e2 = (9 | 3); if (e1 == e2) { fprintf(stderr, "%s => %d\n", "9 | 3", e2); } else { fprintf(stderr, "line %d: %s: %d expected, but got %d\n", 123, "9 | 3", e1, e2); exit(1); } } while (0);
  do { int e1 = (5); int e2 = (6 ^ 3); if (e1 == e2) { fprintf(stderr, "%s => %d\n", "6 ^ 3", e2); } else { fprintf(stderr, "line %d: %s: %d expected, but got %d\n", 124, "6 ^ 3", e1, e2); exit(1); } } while (0);
  do { int e1 = (2); int e2 = (6 & 3); if (e1 == e2) { fprintf(stderr, "%s => %d\n", "6 & 3", e2); } else { fprintf(stderr, "line %d: %s: %d expected, but got %d\n", 125, "6 & 3", e1, e2); exit(1); } } while (0);
  do { int e1 = (0); int e2 = (6 & 0); if (e1 == e2) { fprintf(stderr, "%s => %d\n", "6 & 0", e2); } else { fprintf(stderr, "line %d: %s: %d expected, but got %d\n", 126, "6 & 0", e1, e2); exit(1); } } while (0);
  do { int e1 = (3); int e2 = (({ int x; int y; x=y=3; x; })); if (e1 == e2) { fprintf(stderr, "%s => %d\n", "({ int x; int y; x=y=3; x; })", e2); } else { fprintf(stderr, "line %d: %s: %d expected, but got %d\n", 128, "({ int x; int y; x=y=3; x; })", e1, e2); exit(1); } } while (0);
  do { int e1 = (3); int e2 = (({ int x; int y; x=y=3; y; })); if (e1 == e2) { fprintf(stderr, "%s => %d\n", "({ int x; int y; x=y=3; y; })", e2); } else { fprintf(stderr, "line %d: %s: %d expected, but got %d\n", 129, "({ int x; int y; x=y=3; y; })", e1, e2); exit(1); } } while (0);
  do { int e1 = (45); int e2 = (({ int x=0; int y=0; do { y=y+x; x=x+1; } while (x < 10); y; })); if (e1 == e2) { fprintf(stderr, "%s => %d\n", "({ int x=0; int y=0; do { y=y+x; x=x+1; } while (x < 10); y; })", e2); } else { fprintf(stderr, "line %d: %s: %d expected, but got %d\n", 131, "({ int x=0; int y=0; do { y=y+x; x=x+1; } while (x < 10); y; })", e1, e2); exit(1); } } while (0);
  do { int e1 = (60); int e2 = (({ int sum=0; int i; for (i=10; i<15; i=i+1) sum = sum + i; sum;})); if (e1 == e2) { fprintf(stderr, "%s => %d\n", "({ int sum=0; int i; for (i=10; i<15; i=i+1) sum = sum + i; sum;})", e2); } else { fprintf(stderr, "line %d: %s: %d expected, but got %d\n", 135, "({ int sum=0; int i; for (i=10; i<15; i=i+1) sum = sum + i; sum;})", e1, e2); exit(1); } } while (0);
  do { int e1 = (89); int e2 = (({ int i=1; int j=1; for (int k=0; k<10; k=k+1) { int m=i+j; i=j; j=m; } i;})); if (e1 == e2) { fprintf(stderr, "%s => %d\n", "({ int i=1; int j=1; for (int k=0; k<10; k=k+1) { int m=i+j; i=j; j=m; } i;})", e2); } else { fprintf(stderr, "line %d: %s: %d expected, but got %d\n", 136, "({ int i=1; int j=1; for (int k=0; k<10; k=k+1) { int m=i+j; i=j; j=m; } i;})", e1, e2); exit(1); } } while (0);
  printf("OK\n");
  return 0;
}