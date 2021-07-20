#!/bin/bash
#for filename in `find ./tests/ | egrep '\.cpp'`; 
#do 
 # gcov -n -o ./tests/bin/ $filename > /dev/null; 
#done

for file in ./tests/bin/*; do
    gcov $file.gcda
done