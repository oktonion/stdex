#!/bin/bash
#for filename in `find ./tests/ | egrep '\.cpp'`; 
#do 
 # gcov -n -o ./tests/bin/ $filename > /dev/null; 
#done



mkdir ./tests/tmp

cp ./tests/bin/* ./tests/tmp/
cp *.gcda ./tests/bin/
cp *.gcno ./tests/bin/

cd ./tests/bin/

for file in ./../../tests/tmp/*; do
    gcov -abcfu ./$file.gcda
done