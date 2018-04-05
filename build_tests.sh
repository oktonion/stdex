mkdir ./tests/bin
for file in ./tests/*.cpp; do
  filename=$(basename -- "$file")
  filename="${filename%.*}"
  echo "compiling test $filename"
  $COMPILER -o "./tests/bin/$filename" $file
done