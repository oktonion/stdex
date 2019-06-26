mkdir .\tests\bin
mkdir .\tests\obj

setlocal enabledelayedexpansion

set "build_ok=true"

for /f %%f in ('dir /b ".\tests\*.cpp"') do (
  echo "compiling test Borland C++ %%~nf"
  bcc32 -w -Q -n.\tests\obj\ -L.\stdex\lib\ -I%cd%\pthread\  -c".\tests\%%f"
  IF ERRORLEVEL 1 (
    set "build_ok=false"
  )

  if /I "%build_ok%" NEQ "0" (
    bcc32 -w -Q -L.\stdex\lib\ -I%cd%\pthread\ stdex.lib -n.\tests\bin\ .\tests\obj\%%~nf.obj
    IF ERRORLEVEL 1 (
      set "build_ok=false"
    )
  )
)

if "%build_ok%" == "false" (
  echo "tests build failed"
  exit /B 1
)

del /Q .\tests\obj\*.obj