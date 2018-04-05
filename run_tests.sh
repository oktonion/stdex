for file in ./tests/bin/*; do
  chmod +x "$file"
  filename=$(basename -- "$file")
  filename=$([[ "$filename" = *.* ]] && echo "${filename%.*}" || echo "$filename")
  echo "running test $filename"
  "$file"
done