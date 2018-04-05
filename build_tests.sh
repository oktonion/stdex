mkdir ./tests/bin
for file in ./tests/*.cpp; do
  filename="${file%.*}"
  echo "compiling test $filename"
  $COMPILER -o ./tests/bin/$filename $file
done