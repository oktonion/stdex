#!/bin/sh

COMPILER=g++

$COMPILER --version

mkdir ./obj


build_ok=1


for file in ./src/*.cpp; do
  filename=$(basename -- "$file")
  filename="${filename%.*}"
  echo "compiling $filename"
  if $COMPILER -ftemplate-depth-900 -O3 -pedantic $exclude_warn -lstdlib -c "$file" -o "./obj/$filename.o"; then
    echo ""
  else
    build_ok=0
  fi
done

if [ $build_ok -eq 0 ]; then
  echo "libstdex.a build failed"
  exit 1
fi

mkdir ./lib

libname="rcs ./lib/libstdex.a"

for file in ./obj/*.o; do
  filename=$(basename -- "$file")
  filename="${filename%.*}"
  echo "archiving $filename to libstdex.a"
  wlib $libname +$filename
done

ls ./obj

ls ./lib

echo "lib done"
echo "contains:"
ar -t "./lib/libstdex.a"


