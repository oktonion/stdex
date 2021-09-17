#!/bin/bash
#for filename in `find ./tests/ | egrep '\.cpp'`; 
#do 
 # gcov -n -o ./tests/bin/ $filename > /dev/null; 
#done

cp *.gcda ./tests/bin/
cp *.gcno ./tests/bin/


for file in ./tests/bin/*; do
    gcov -abcfu $file.gcda
done