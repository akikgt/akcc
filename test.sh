#!/bin/bash
try() {
    expected="$1"
    input="$2"

    ./xcc "$input" > tmp.s
    gcc -o tmp tmp.s tmp-plus.o test-alloc.o -no-pie
    ./tmp
    actual="$?"

    if [ "$actual" = "$expected" ]; then
        echo "$input => $actual"
    else
        echo "$expected expected, but got $actual"
        exit 1
    fi
}

echo 'int plus(int a, int b) { printf("Function call test: %d\n", a + b); return a + b; }' | gcc -xc -c -o tmp-plus.o - 


# passing array test
try 65 'int demo(char a[][2]) {
        return a[2][1];
    }
    int main() {
        char b[3][2];
        b[2][1] = 65;
        return demo(b);
    }
'
try 1 'int main() { char *name = "test\"aaa\"test\n"; printf(name); return 1;} '
try 3 'int main() { int i = 100; return (i - 100) ? 2 : 3;} '
try 3 'int main() { return !(3-2) || 3;} '
try 3 'int main() { return 1 * 100 * 0 || 0 || (1 + 5 * 3 - 15 + 2);} '
try 3 'int main() { int i = 15; return i % 7 && 15 / 5;} '
#post-increment/decrement test
# try 1 'int main() 
#     {
#     int a[2];
#     a[1] = a[1] + 1;
#     a[1] += 2;
#     int b = ++a[1];
#     return b; 
#     } 
# '
try 10 'int main() 
    {
    int a[2][3][5];
    a[1][1][3] = -256;
    a[1][2][3] = 1000;
    a[1][2][4] = 10;
    return a[1][2][4];
    } 
'
try 10 'int main() 
    {
    int a[2];
    a[0] = 10;
    a[1] = 20;
    int b = 0;
    return a[b++]; 
    } 
'
try 3 'int main() 
    {
    int a;
    a = 3;
    int b = a--;
    printf("a is %d\n", a);
    return b; 
    } 
'
try 2 'int main() { int a = 1; int b = ++a; return b;} '
try 2 'int main() { int a = 3; int b = --a; return b;} '
try 1 'int main() { int a = 0; a += 1; return a;} '
try 1 'int main() { int a = 2; a -= 1; return a;} '
try 8 'int main() { int a = 4; a *= 2; return a;} '
try 2 'int main() { int a = 4; a /= 2; return a;} '
try 1 'int main() { int a = 5; a %= 2; return a;} '
try 5 'int main() {
    int *p = malloc(sizeof(char));
    *p = 5;
    return *p;
    }'

#char test
try 1 'int main() { char *a; a = "abc"; return 1;} '
try 1 'int main() { printf("yattaze\n"); return 1;} '
try 0 'int main() { char a; return a;} '
try 1 '
int main() {
    char x[3];
    x[0] = -3;
    x[1] = 2;
    int y;
    y = 4;
    return x[0] + y; 
}
'
try 65 'char b; int main(char arg) { arg = 5; char a = 65; return a;} '
#global variable
try 0 'int a; int main() { return a;} '
try 2 'int a; int main() { a = 2; return a;} '
try 100 'int a[2]; int main() { a[1] = 100; return a[1];} '
#pointer test
try 8 'int main(int a[100]) { int *p; return sizeof(p + 2); }'
try 4 'int main() { int i; return sizeof i; }'
try 8 'int main() { 
int a[10]; 
*a = 8;
return *a; }'
try 40 'int main() { int a[10]; return sizeof a; }'
try 3 'int main() { int a[10]; a[2] = 3; return a[1*2]; }'
try 3 '
int main() {
int a[2];
*a = 1;
*(1 + a) = 2;
int *p;
p = a;
return *p + *(p + 1); }
'
try 20 'int main() { int a[1][5][10]; int b = 20; return b; }'
try 0 'int main(int arg, int **argv) {int ****p; return 0; } '
try 8 '
    int main() {
        int **p;
        p = 0;
        p = 1 + p - 10 + 10;
        int i = p;
        return i;
    }
'
try 3 '
    int main() {
        int *p;
        alloc1(&p, 3);
        return *p;
    }
'

try 8 '
    int main() {
        int *p;
        alloc4(&p, 1, 2, 4, 8);
        int *q;
        q = p + 4;
        q = q - 1;
        return *q;
    }
'

try 3 '
    int main() {
        int x;
        x = 1;
        int *y;
        y = &x;
        x = 3;
        return *y;
        }'

try 0 'int main() { return 0;}'
try 42 'int main() { return 42;}'
try 100 'int main() { return 10 + 20 + 30 + 20 / 2 * 4;}'
try 16 'int main() {int i = 1; while(i < 16) i = i * 2; i;}'
try 3 'int main() {for(int i = 1; i < 3; i = i + 1) 1; i;}'
try 6 'int main() {int i = 0; for (; i < 5 ;) i = i + 3; i;}'
try 100 'int main() { int a = 100; return a;}'

try 5 'int main() {plus(2, 3);}'
try 5 'int main() {plus(2, 1 + 1 + 1);}'

try 2 'int two() {return 2;} int main() { return two();}'
try 10 'int my_mul(int a, int b) {return a * b;} int main() { return my_mul(5, 2);}'
try 3 'int my_sub(int a, int b) {return a - b;} int main() { return my_sub(5, 2);}'

try 10 'int my_mul(int a, int b, int c) {return a * b * c;} int main() { return my_mul(5, 2, 1);}'

#fibonacci test
try 55 '
    int fib(int a, int b, int count) {
        if (count == 1) {
            return a;
        }
        return fib(b, a+b, count - 1);
    }
    int main() {
    return fib(1, 1, 10);
    }
    '
#factorial test
try 120 '
    int factorial(int n) {
        if (n == 1)
            return 1;
        return n * factorial(n-1);
    }
    int main() {
        return factorial(5);
    }
'


try 1 'int main() { return 100 % 3; }'
try 1 '
    int main() {
        int x = 10;
        if (x % 2 == 1)
            return 0;
        else
            return 1;
    }
'

try 6 '
int main() {
    int a[5];
    for (int i = 0; i < 5; i = i + 1) {
        a[i] = i * 2;
    }

    return a[3];
}
'
echo OK