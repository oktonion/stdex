$COMPILER --version

mkdir ./obj


build_ok=1

for file in ./src/*.cpp; do
  filename=$(basename -- "$file")
  filename="${filename%.*}"
  echo "compiling $filename"
  if ! $COMPILER -std=c++98 -c "$file" -o "./obj/$filename.o"; then
    build_ok=0
  fi
done

if [ $build_ok -eq 0 ]; then
  echo "libstdex.a build failed"
  exit 1
fi

mkdir ./lib

ar_args="rcs ./lib/libstdex.a"

for file in ./obj/*.o; do
  filename=$(basename -- "$file")
  filename="${filename%.*}"
  echo "archiving $filename to libstdex.a"
  ar_args="$ar_args $file"
done

ls ./obj
echo "ar $ar_args"
ar $ar_args

ls ./lib

echo "lib done"
echo "contains:"
ar -t "./lib/libstdex.a"

