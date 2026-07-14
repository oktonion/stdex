mkdir ./tests/bin

build_ok=1
tests_failed="unsuccessful tests:"
exclude_warn=""

if test -n "${COMPILER-}"; then
	libname="stdex-$COMPILER"
else
	COMPILER=g++
	libname="stdex"
fi

if [[ $COMPILER = *"clang"* ]]; then
  exclude_warn="-Wno-c++11-long-long -Wno-non-literal-null-conversion"
else
  exclude_warn="-Wno-long-long"
fi

for file in ./tests/*.cpp; do
  filename=$(basename -- "$file")
  filename="${filename%.*}"
  echo "compiling test c++03 $filename"
  output=$(($COMPILER -pedantic -O3 $exclude_warn $file -L./stdex/lib/ -l$libname -lm -o "./tests/bin/$filename") 2>&1)
  if [[ $? -ne 0 ]]; then
    if [[ $filename == *"fail"* ]]; then
      echo "failed as expected"
    else
      build_ok=0
      tests_failed="$tests_failed $filename;"
      echo "$output"
    fi
  else
    if [[ $filename == *"fail"* ]]; then
      build_ok=0
      tests_failed="$tests_failed $filename;"
      echo "not failed as expected"
    fi
  fi
done

if [ $build_ok -eq 0 ]; then
  echo "$tests_failed"
  exit 3
fi

if [[ $COMPILER != *"qcc"* ]]; then

for file in ./tests/*.cpp; do
  filename=$(basename -- "$file")
  filename="${filename%.*}"
  echo "compiling test c++98 $filename"
  output=$(($COMPILER -std=c++98 -pedantic $exclude_warn $file -L./stdex/lib/ -l$libname -lm -o "./tests/bin/$filename") 2>&1)
  if [[ $? -ne 0 ]]; then
    if [[ $filename == *"fail"* ]]; then
      echo "failed as expected"
    else
      build_ok=0
      tests_failed="$tests_failed $filename;"
      echo $output
    fi
  else
    if [[ $filename == *"fail"* ]]; then
      build_ok=0
      tests_failed="$tests_failed $filename;"
      echo "not failed as expected"
    fi
  fi
done

fi

if [ $build_ok -eq 0 ]; then
  echo "$tests_failed"
  exit 98
fi