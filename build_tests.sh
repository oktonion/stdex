mkdir ./tests/bin

build_ok=1
exclude_warn=""
tests_failed="failed tests:"
build_libs="-lrt"

$COMPILER -v

if [[ $COMPILER = *"clang"* ]]; then
  exclude_warn="-Wno-c++11-long-long -Wno-non-literal-null-conversion"
else
  exclude_warn="-Wno-long-long"
fi

case "$(uname -s)" in

   Darwin)
     echo 'Mac OS X'
     build_libs="-lpthread"
     ;;

   Linux)
     echo 'Linux'
     build_libs="-lrt -lpthread"
     ;;

   CYGWIN*|MINGW32*|MSYS*)
     echo 'MS Windows'
     exit 13
     ;;

   # Add here more strings to compare
   # See correspondence table at the bottom of this answer

   *)
     echo 'other OS' 
     exit 13
     ;;
esac

if [[ $COMPILER = *"g++-4."* ]]; then
  echo "c++03 option is not supported"
else
  for file in ./tests/*.cpp; do
    filename=$(basename -- "$file")
    filename="${filename%.*}"
    echo "compiling test c++03 $filename"
    if ! $COMPILER -std=c++03 -pedantic $exclude_warn $CODE_COVERAGE_FLAGS $file -L./stdex/lib/ -lstdex $build_libs $CODE_COVERAGE_LIBS -o "./tests/bin/$filename"; then
      if [[ $filename == *"fail"* ]]; then
        # nothing
      else
        build_ok=0
        tests_failed="$tests_failed $filename;"
      fi
    fi
  done
fi

if [ $build_ok -eq 0 ]; then
  echo "$tests_failed"
  exit 3
fi

tests_failed="failed tests for c++98:"

for file in ./tests/*.cpp; do
  filename=$(basename -- "$file")
  filename="${filename%.*}"
  echo "compiling test c++98 $filename"
  if ! $COMPILER -std=c++98 -pedantic $exclude_warn $CODE_COVERAGE_FLAGS $file -L./stdex/lib/ -lstdex $build_libs $CODE_COVERAGE_LIBS -o "./tests/bin/$filename"; then
    if [[ $filename == *"fail"* ]]; then
      # nothing
    else
      build_ok=0
      tests_failed="$tests_failed $filename;"
    fi
  fi
done

if [ $build_ok -eq 0 ]; then
  echo "$tests_failed"
  exit 98
fi