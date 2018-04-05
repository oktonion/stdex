for file in ./tests/bin/*; do
  chmod +x "$file"
  echo "running test $file"
  "$file"
done