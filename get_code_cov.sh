#!/bin/bash
for filename in `find ./tests/ | egrep '\.cpp'`; 
do 
  gcov -n -o . $filename > /dev/null; 
done