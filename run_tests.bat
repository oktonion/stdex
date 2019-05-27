setlocal enabledelayedexpansion

set "run_ok=true"
for /f %%f in ('dir /b ".\tests\bin\*.exe"') do (
  echo "running test %%~nf..."
  
  .\tests\bin\%%f --duration

  IF ERRORLEVEL 1 (
    set "run_ok=false"
    echo "...failed with %ERRORLEVEL%."
  ) else (
    echo "...ok."
  )
)

if "%run_ok%" == "false" (
  echo "tests run failed"
  exit /B 1
)