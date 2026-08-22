run_ok=0
tests_failed="unsuccessful tests:"

for file in ./tests/bin/*; do
  if file "$file" | grep -q "exec"
  then
    chmod +x "$file"

    filename=$(basename -- "$file")
    filename=$([[ "$filename" = *.* ]] && echo "${filename%.*}" || echo "$filename")
    echo "$(date -u) running test $filename..."
    output=$(($file --duration) 2>&1)
    run_result=$?
    if [ $run_result -eq 0 ]; then
      echo "$(date -u) ...ok."
    else
      run_ok=1
      echo "$output"
      echo "...failed with $run_result."
      tests_failed="$tests_failed $filename;"
    fi
  else
    echo "$file is not valid executable - skip"
  fi
done

if [ $run_ok -eq 1 ]; then
  echo "$tests_failed"
fi

exit $run_ok