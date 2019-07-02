mkdir ./tests/bin

build_ok=1
tests_failed="failed tests:"
exclude_warn=""
COMPILER=qcc

if [[ $COMPILER = *"clang"* ]]; then
  exclude_warn="-Wno-c++11-long-long -Wno-non-literal-null-conversion"
else
  exclude_warn="-Wno-long-long"
fi

for file in ./tests/*.cpp; do
  filename=$(basename -- "$file")
  filename="${filename%.*}"
  echo "compiling test c++03 $filename"
  if ! $COMPILER -pedantic $exclude_warn $file -L./stdex/lib/ -lstdex -lm -o "./tests/bin/$filename"; then
    build_ok=0
    tests_failed="$tests_failed $filename;"
  fi
done

if [ $build_ok -eq 0 ]; then
  echo "$tests_failed"
  exit 3
fi