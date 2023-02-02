mkdir ./tests/bin

build_ok=1
exclude_warn=""
tests_failed="unsuccessful tests:"
build_libs="-lrt"
compiler_options="-O3"

case "$(uname -s)" in

   Darwin)
     echo 'OS: Mac OS X'
     build_libs="-lpthread"
	 compiler_of_choice=clang
   ;;

   Linux)
     echo 'OS: Linux'
     build_libs="-lrt -lpthread"
	 compiler_of_choice=g++
   ;;

   CYGWIN*|MINGW32*|MSYS*|MINGW64*)
     echo 'OS: MS Windows'
     compiler_options="-I./pthread/ -fms-extensions -Wno-language-extension-token"
	 compiler_of_choice=g++
   ;;
   
   QNX)
     echo 'OS: QNX' 
     build_libs="-lm"
	 compiler_of_choice=qcc
   ;;
   
   *)
     echo 'OS: unknown'
	 compiler_of_choice=g++
   ;;
esac

if [ -z ${COMPILER} ]; then
  output=$( (g++ -v) 2>&1)
  case "$output" in
    *cannot*|*such*|*directory*)
	  echo "cannot find 'g++' compiler"
    ;;
	*)
	  compiler_of_choice=g++
	;;
  esac
  
  echo "COMPILER var is not set, setting to '$compiler_of_choice'"
  COMPILER=$compiler_of_choice
fi

$COMPILER -v

case "$COMPILER" in 
  *clang*)
    exclude_warn="-Wno-c++11-long-long -Wno-non-literal-null-conversion"
  ;;
  *)
    exclude_warn="-Wno-long-long"
  ;;
esac


for file in ./tests/*.cpp; do
  filename=$(basename -- "$file")
  filename="${filename%.*}"
  echo "$(date): compiling test c++ recent $filename"
  output=$( ($COMPILER $compiler_options -pedantic $exclude_warn $CODE_COVERAGE_FLAGS $file -L./stdex/lib/ -lstdex $build_libs $CODE_COVERAGE_LIBS -o "./tests/bin/$filename") 2>&1)
  if [ $? -ne 0 ]; then
    case "$filename" in
      *fail*)
        echo "failed as expected"
      ;;
      *)
        build_ok=0
        tests_failed="$tests_failed $filename;"
        echo $output
      ;;
    esac
  else
    case "$filename" in
      *fail*)
        build_ok=0
        tests_failed="$tests_failed $filename;"
        echo "not failed as expected"
      ;;
    esac
  fi
done


if [ $build_ok -eq 0 ]; then
  echo "$tests_failed"
fi


case "$COMPILER" in 
  g++-4.*|g++-3.*)
    echo "outdated compiler: c++03 option is not supported"
  ;;
  *)
    tests_failed="unsuccessful tests C++03:"
    compiler_options="-std=c++03 -O3"
    for file in ./tests/*.cpp; do
      filename=$(basename -- "$file")
      filename="${filename%.*}"
      echo "$(date): compiling test c++03 $filename"
      output=$( ($COMPILER $compiler_options -pedantic $exclude_warn $CODE_COVERAGE_FLAGS $file -L./stdex/lib/ -lstdex $build_libs $CODE_COVERAGE_LIBS -o "./tests/bin/$filename") 2>&1)
      if [ $? -ne 0 ]; then
      case "$output" in
      *error*c++03*)
        echo "c++03 option is not supported"
      break
      ;;
    esac
    
        case "$filename" in 
          *fail*)
            echo "failed as expected"
          ;;
          *)
            build_ok=0
            tests_failed="$tests_failed $filename;"
            echo $output
          ;;
        esac
      else
        case "$filename" in 
          *fail*)
            build_ok=0
            tests_failed="$tests_failed $filename;"
            echo "not failed as expected"
          ;;
        esac
      fi
    done
  ;;
esac

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
     compiler_options="-std=c++98 -O3"
   ;;
esac

if [ -z ${CODE_COVERAGE_LIBS+x} ]; then
  echo "c++98 build"
else
  echo "c++98 coverage build"
  compiler_options="-std=c++98 -g -O0"
fi

rm ./tests/bin/*

tests_failed="failed tests for c++98:"

for file in ./tests/*.cpp; do
  filename=$(basename -- "$file")
  filename="${filename%.*}"
  echo "$(date): compiling test c++98 $filename"
  output=$( ($COMPILER $compiler_options -pedantic $exclude_warn $CODE_COVERAGE_FLAGS $file -L./stdex/lib/ -lstdex $build_libs $CODE_COVERAGE_LIBS -o "./tests/bin/$filename") 2>&1)
  if [ $? -ne 0 ]; then
    case "$filename" in 
      *fail*)
        echo "failed as expected"
      ;;
      *)
        build_ok=0
        tests_failed="$tests_failed $filename;"
        echo $output
      ;;
    esac
  else
    case "$filename" in 
      *fail*)
        build_ok=0
        tests_failed="$tests_failed $filename;"
        echo "not failed as expected"
      ;;
    esac
  fi
done

if [ $build_ok -eq 0 ]; then
  echo "$tests_failed"
  exit 98
fi