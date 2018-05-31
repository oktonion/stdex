setlocal enabledelayedexpansion

set run_ok=0
for /f %%f in ('dir /b ".\tests\bin\*.exe"') do (
  echo "running test %%~nf..."
  .\tests\bin\%%f
  if /I "%ERRORLEVEL%" NEQ "0" (
    %run_ok%=-1
    echo "...failed with %ERRORLEVEL%."
  ) else (
    echo "...ok."
  )
)

exit %run_ok%