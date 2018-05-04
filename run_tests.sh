run_ok=0
for file in ./tests/bin/*; do
  chmod +x "$file"
  filename=$(basename -- "$file")
  filename=$([[ "$filename" = *.* ]] && echo "${filename%.*}" || echo "$filename")
  echo "running test $filename..."
  run_result="$file"
  if [ $run_result -eq 0 ]; then
    echo "...ok."
  else
    run_ok=-1
    echo "...failed at line $run_result."
  fi 
done

exit $run_ok