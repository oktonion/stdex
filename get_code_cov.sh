#!/bin/bash
mkdir ./tests/coverage
for filename in `find ./tests/ | egrep '\.cpp'`; 
do 
  gcov -n -o ./tests/bin/ $filename > /dev/null; 
done