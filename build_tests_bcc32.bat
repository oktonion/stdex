mkdir .\tests\bin
mkdir .\tests\obj

setlocal enabledelayedexpansion

set "build_ok=true"

for /f %%f in ('dir /b ".\tests\*.cpp"') do (
  echo "compiling test Borland C++ %%~nf"
  bcc32 -w-inl -w-ccc -tWM -Q -n.\tests\obj\ -L.\stdex\lib\ -I%cd%\pthread\ -c ".\tests\%%f"
  IF ERRORLEVEL 1 (
    set "build_ok=false"
  )

  if /I "%build_ok%" NEQ "0" (
    bcc32 -w-inl -w-ccc -tWM -Q -L.\stdex\lib\ -lap -I%cd%\pthread\ stdex.lib cw32mt.lib ntdll.lib -o.\tests\bin\%%~nf.exe .\tests\obj\%%~nf.obj
    IF ERRORLEVEL 1 (
      set "build_ok=false"
      exit /B 1
    )
  )
)

if "%build_ok%" == "false" (
  echo "tests build failed"
  exit /B 1
)

del /Q .\tests\obj\*.obj