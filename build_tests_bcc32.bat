mkdir .\tests\bin
mkdir .\tests\obj

setlocal enabledelayedexpansion

set "build_ok=1"


for /f %%f in ('dir /b ".\tests\*.cpp"') do (
  echo "compiling test Borland C++ %%~nf"
  bcc32 -x -w-inl -w-ccc -tCM -Q -n.\tests\obj\ -L.\stdex\lib\ -I%cd%\pthread\ -c ".\tests\%%f"
  IF ERRORLEVEL 1 (
    set "build_ok=0"
    exit /B 1
  )

  if "%build_ok%"=="1" (
    bcc32 -x -w-inl -w-ccc -tCM -Q -L.\stdex\lib\ -lap -I%cd%\pthread\ -e.\tests\bin\%%~nf.exe stdex.lib cw32mt.lib ntdll.lib .\tests\obj\%%~nf.obj
    IF ERRORLEVEL 1 (
      set "build_ok=0"
      exit /B 1
    )
  )
)

if /I "%build_ok%" NEQ "1" (
  echo "tests build failed"
  exit /B 1
)

echo "tests build ok"

del /Q .\tests\obj\*.obj