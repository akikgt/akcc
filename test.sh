#!/bin/bash
try() {
    expected="$1"
    input="$2"

    ./xcc "$input" > tmp.s
    gcc -o tmp tmp.s tmp-plus.o
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

# try 0 'int main {}'
echo OK