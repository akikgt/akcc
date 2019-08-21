#!/bin/bash
make
./akcc -file ./sample/sample.c  > tmp-sample.s
gcc -o tmp tmp-sample.s -no-pie
./tmp