#!/bin/sh

mkdir ./obj
build_ok=1
compiler_options="-std=c++98 -O3"

if [ -z ${CODE_COVERAGE_LIBS+x} ]; then
  echo "c++98 build"
else
  echo "c++98 coverage build"
  compiler_options="-std=c++98 -g"
fi

case "$(uname -s)" in

   Darwin)
     echo 'OS: Mac OS X'
	 compiler_of_choice=clang++
     ;;

   Linux)
     echo 'OS: Linux'
	 compiler_of_choice=g++
     ;;

   CYGWIN*|MINGW32*|MSYS*|MINGW64*)
     echo 'OS: MS Windows'
     compiler_options="-I../pthread/ -fms-extensions"
	 compiler_of_choice=g++
     ;;
   
   QNX)
     echo 'OS: QNX'
	 compiler_of_choice=qcc
     ;;

   # Add here more strings to compare
   # See correspondence table at the bottom of this answer

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

for file in ./src/*.cpp; do
  filename=$(basename -- "$file")
  filename="${filename%.*}"
  echo "compiling $filename"
  if ! $COMPILER $compiler_options -c "$file" -o "./obj/$filename.o"; then
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


