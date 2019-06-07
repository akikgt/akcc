#!/bin/bash
try() {
    expected="$1"
    input="$2"

    ./xcc "$input" > tmp.s
    gcc -o tmp tmp.s
    ./tmp
    actual="$?"

    if [ "$actual" = "$expected" ]; then
        echo "$input => $actual"
    else
        echo "$expected expected, but got $actual"
        exit 1
    fi
}

# simple number
try 0 '0;'
try 42 '42;'
# add and sub
try 21 '5+20-4;'
try 41 ' 12 + 34 - 5 ;'
# mul and div
try 47 '5+6*7;'
try 15 '5*(9-6);'
try 4 '(3+5)/2;'
# unary operator
try 5 '-10+15;'
try 5 '-(-5);'
try 0 '-(-(-(-0)));'
#equality
try 1 '1 == 1;'
try 0 '1 == 2;'
try 1 '1 != 2;'
try 0 '1 == 2;'
#relational
try 1 '1 < 2;'
try 0 '10 < 2;'
try 1 '-1 <= 2;'
try 1 '2 <= 2;'
try 0 '5 <= 2;'
try 1 '5 > 3;'
try 0 '-5 > 3;'
try 1 '500 >= 300;'
try 0 '-128 >= 127;'
#multi statements
try 1 '5;4;3;2;1;'
try 2 '5;4;3;2;'
#variable
try 3 'a=3;'
#assignment
try 10 'z=2; z + 8;'
try 10 'a=3; z=2; a + z + 5;'
try 2 'a = 1; a = a + 1;'
#return
try 2 'return 2;'
try 3 '1 + 1; a=100; return 3; 100;'
#multi-character variable
try 7 'abc=7; return abc;'
try 3 'abc=7; de = 1; abc=de+1; return abc + de;'
#if else statement
try 2 'if(1) 2;'
try 0 'if(0) 3;'
try 3 'if(1) 3; else 7;'
try 7 'if(0) 3; else 7;'
try 8 'if(1 + 1 >= 5) 3; else 8;'
#while
try 4 'i = 0; while(i <= 3) i = i + 1;'
try 16 'i = 1; while(i < 16) i = i * 2; i;'
#for
try 3 'for(i = 1; i < 3; i = i + 1) 1; i;'
try 6 'i = 0; for (; i < 5 ;) i = i + 3; i;'
#compound statements
try 0 '{}'
try 8 '{1;2;3;4;5;6; 2 * 2 * 2;}'
try 8 '{1; i = 0; while(i <= 3) i = i + 1; 2;3;4;5;6; 2 * 2 * 2;}'
echo OK