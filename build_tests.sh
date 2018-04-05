mkdir ./tests/bin
for file in ./tests/*.cpp; do
  filename=$(basename -- "$fullfile")
  filename="${filename%.*}"
  echo "compiling test $filename"
  $COMPILER -o "./tests/bin/$filename" $file
done