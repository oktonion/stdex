mkdir ./tests/bin

build_ok=1
exclude_warn=""
tests_failed="unsuccessful tests:"
build_libs="-lrt"
compiler_options="-std=c++03 -O3"

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

   CYGWIN*|MINGW32*|MSYS*|MINGW64*)
     echo 'MS Windows'
     compiler_options="-I./pthread/ -fms-extensions -Wno-language-extension-token"
     ;;

   *)
     echo 'other OS' 
     ;;
esac

if [[ $COMPILER = *"g++-4."* ]]; then
  echo "c++03 option is not supported"
elif [[ $COMPILER = *"g++-3."* ]]; then
  echo "c++03 option is not supported"
else
  for file in ./tests/*.cpp; do
    filename=$(basename -- "$file")
    filename="${filename%.*}"
    echo "$(date): compiling test c++03 $filename"
    output=$(($COMPILER $compiler_options -pedantic $exclude_warn $CODE_COVERAGE_FLAGS $file -L./stdex/lib/ -lstdex $build_libs $CODE_COVERAGE_LIBS -o "./tests/bin/$filename") 2>&1)
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
  exit 3
fi

case "$(uname -s)" in

   CYGWIN*|MINGW32*|MSYS*|MINGW64*)
     echo 'MS Windows'
     compiler_options="-I./pthread/ -fms-extensions -Wno-language-extension-token"
     ;;


   *) 
   compiler_options="-std=c++98"
     ;;
esac

tests_failed="failed tests for c++98:"

for file in ./tests/*.cpp; do
  filename=$(basename -- "$file")
  filename="${filename%.*}"
  echo "$(date): compiling test c++98 $filename"
  output=$(($COMPILER $compiler_options -pedantic $exclude_warn $CODE_COVERAGE_FLAGS $file -L./stdex/lib/ -lstdex $build_libs $CODE_COVERAGE_LIBS -o "./tests/bin/$filename") 2>&1)
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
  exit 98
fi