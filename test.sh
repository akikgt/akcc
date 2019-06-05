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
try 0 0
try 42 42
# add and sub
try 21 '5+20-4'
try 41 ' 12 + 34 - 5 '
# mul and div
try 47 '5+6*7'
try 15 '5*(9-6)'
try 4 '(3+5)/2'
# unary operator
try 5 '-10+15'
try 5 '-(-5)'
try 0 '-(-(-(-0)))'
#equality
try 1 '1 == 1'
try 0 '1 == 2'
try 1 '1 != 2'
try 0 '1 == 2'
#relational
try 1 '1 < 2'
try 0 '10 < 2'
try 1 '-1 <= 2'
try 1 '2 <= 2'
try 0 '5 <= 2'

echo OK