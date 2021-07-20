#!/bin/bash
for filename in `find . | egrep '\.h*'`; 
do 
  gcov -n -o . $filename > /dev/null; 
done