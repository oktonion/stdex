mkdir ./tests/bin

build_ok=1

for file in ./tests/*.cpp; do
  filename=$(basename -- "$file")
  filename="${filename%.*}"
  echo "compiling test $filename"
  if ! $COMPILER -std=c++0x -L./stdex/lib/ -lstdex -o "./tests/bin/$filename" $file; then
    build_ok=0
  fi
done

if [ $build_ok -eq 0 ]; then
  echo "tests build failed"
  exit 3
fi

for file in ./tests/*.cpp; do
  filename=$(basename -- "$file")
  filename="${filename%.*}"
  echo "compiling test $filename"
  if ! $COMPILER -std=98 -L./stdex/lib/ -lstdex -o "./tests/bin/$filename" $file; then
    build_ok=0
  fi
done

if [ $build_ok -eq 0 ]; then
  echo "tests build failed for c++98"
  exit 98
fi