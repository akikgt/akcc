#!/bin/bash
make
./xcc -file sample  > tmp-sample.s
gcc -o tmp tmp-sample.s -no-pie
./tmp