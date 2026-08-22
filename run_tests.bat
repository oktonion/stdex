setlocal enabledelayedexpansion
@ECHO OFF

set true=1==1
set false=1==0

set "tests_failed=unsuccessful tests:"
set run_ok=!true!

for /f %%f in ('dir /b ".\tests\bin\*.exe"') do (
  echo "%date% %time% running test %%~nf..."
  
  ver > nul

  .\tests\bin\%%f --duration && (
    echo "%date% %time% ...ok."
  ) || (
    echo "...failed with !errorlevel!."
    set run_ok=!false!
    set "tests_failed=!tests_failed! %%~nf"
  )
)

if !run_ok!==!false! (
  echo !tests_failed!
  exit /B 1
)