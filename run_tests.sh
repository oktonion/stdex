run_ok=0
tests_failed="failed tests:"

for file in ./tests/bin/*; do
  chmod +x "$file"
  filename=$(basename -- "$file")
  filename=$([[ "$filename" = *.* ]] && echo "${filename%.*}" || echo "$filename")
  echo "running test $filename..."
  $file
  run_result=$?
  if [ $run_result -eq 0 ]; then
    echo "...ok."
  else
    run_ok=-1
    echo "...failed with $run_result."
    tests_failed="$tests_failed $filename;"
  fi 
done

if [ $run_ok -eq -1 ]; then
  echo "$tests_failed"
fi

exit $run_ok