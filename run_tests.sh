run_ok=0
for file in ./tests/bin/*; do
  chmod +x "$file"
  filename=$(basename -- "$file")
  filename=$([[ "$filename" = *.* ]] && echo "${filename%.*}" || echo "$filename")
  echo "running test $filename..."
  run_result="$file"
  if ! run_result; then
    run_ok=-1
    echo "...failed at line $run_result."
  else
    echo "...ok."
  fi 
done

return run_ok