setlocal enabledelayedexpansion

set "run_ok=1"
for /f %%f in ('dir /b ".\tests\bin\*.exe"') do (
  echo "running test %%~nf..."
  
  .\tests\bin\%%f --duration

  IF ERRORLEVEL 1 (
    set "run_ok=0"
    echo "...failed with %ERRORLEVEL%."
  ) else (
    echo "...ok."
  )
)

if /I "%run_ok%" NEQ "1" (
  echo "tests run failed"
  exit /B 1
)

echo "tests run ok"