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

try 0 'main() { return 0;}'
try 42 'main() { return 42;}'
try 100 'main() { return 10 + 20 + 30 + 20 / 2 * 4;}'
try 4 'main() {i = 0; while(i <= 3) i = i + 1;}'
try 16 'main() {i = 1; while(i < 16) i = i * 2; i;}'
try 3 'main() {for(i = 1; i < 3; i = i + 1) 1; i;}'
try 6 'main() {i = 0; for (; i < 5 ;) i = i + 3; i;}'
try 8 'main() {1; i = 0; while(i <= 3) i = i + 1; 2;3;4;5;6; 2 * 2 * 2;}'
try 200 'main() {if (1 + 1 == 2) { 100; 200;}}'

try 5 'main() {plus(2, 3);}'
try 5 'main() {plus(2, 1 + 1 + 1);}'

try 2 'two() {return 2;} main() { return two();}'
echo OK