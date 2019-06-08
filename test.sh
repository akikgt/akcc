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
try 2 'int main() { int a; a = 2; return a;}'
try 100 'int main() { return 10 + 20 + 30 + 20 / 2 * 4;}'
try 4 'int main() {int i; i = 0; while(i <= 3) i = i + 1;}'
try 16 'int main() {int i; i = 1; while(i < 16) i = i * 2; i;}'
try 3 'int main() {int i; for(i = 1; i < 3; i = i + 1) 1; i;}'
try 6 'int main() {int i; i = 0; for (; i < 5 ;) i = i + 3; i;}'
try 8 'int main() {1; int i; i = 0; while(i <= 3) i = i + 1; 2;3;4;5;6; 2 * 2 * 2;}'
try 200 'int main() {if (1 + 1 == 2) { 100; 200;}}'

try 5 'int main() {plus(2, 3);}'
try 5 'int main() {plus(2, 1 + 1 + 1);}'

try 2 'int two() {return 2;} int main() { return two();}'
try 10 'int my_mul(a, b) {return a * b;} int main() { return my_mul(5, 2);}'
try 3 'int my_sub(a, b) {return a - b;} int main() { return my_sub(5, 2);}'

#fibonacci test
try 55 '
    fib(a,b,count) {
        if (count == 1) {
            return a;
        }
        return fib(b, a+b, count - 1);
    }
    main() {
    return fib(1, 1, 10);
    }
    '
#factorial test
try 120 '
    factorial(n) {
        if (n == 1)
            return 1;
        return n * factorial(n-1);
    }
    main() {
        return factorial(5);
    }
'

#integer type
try 1 'int main() { return 1; }'
echo OK