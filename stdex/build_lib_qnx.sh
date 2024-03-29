#!/bin/sh

if test -n "${COMPILER-}"; then
	libname="libstdex-$COMPILER.a"
else
	COMPILER=g++
	libname="libstdex.a"
fi

$COMPILER --version

mkdir ./obj


build_ok=1


for file in ./src/*.cpp; do
  filename=$(basename -- "$file")
  filename="${filename%.*}"
  echo "compiling $filename"
  if $COMPILER -pedantic -O3 $exclude_warn -lstdlib -c "$file" -o "./obj/$filename.o"; then
    echo ""
  else
    build_ok=0
  fi
done

if [ $build_ok -eq 0 ]; then
  echo "$libname build failed"
  exit 1
fi

mkdir ./lib

ar_args="rcs ./lib/$libname"

for file in ./obj/*.o; do
  filename=$(basename -- "$file")
  filename="${filename%.*}"
  echo "archiving $filename to $libname"
  ar_args="$ar_args $file"
done

ls ./obj
echo "ar $ar_args"
ar $ar_args

ls ./lib

echo "lib done"
echo "contains:"
ar -t "./lib/$libname"


