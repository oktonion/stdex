mkdir .\tests\bin
mkdir .\tests\obj

setlocal enabledelayedexpansion

set "build_ok=1"


for /f %%f in ('dir /b ".\tests\*.cpp"') do (
  echo "compiling test Borland C++ %%~nf"
  bcc32 -X- -w-inl -w-ccc -tWC -Q -n.\tests\obj\ -L.\stdex\lib\ -I%cd%\pthread\ -c ".\tests\%%f"
  set origin_str=%%f
  set replaced_str=!origin_str:fail=_!
  IF ERRORLEVEL 1 (
    if not "%origin_str%"=="%replaced_str%" set "build_ok=0"
  )

  if "%build_ok%"=="1" (
    bcc32 -X- -w-inl -w-ccc -tWC -Q -L.\stdex\lib\ -lap -I%cd%\pthread\ -e.\tests\bin\%%~nf.exe stdex.lib cw32mt.lib ntdll.lib .\tests\obj\%%~nf.obj
    set origin_str=%%f
    set replaced_str=!origin_str:fail=_!
    IF ERRORLEVEL 1 (
      if not "%origin_str%"=="%replaced_str%" set "build_ok=0"
    )
  )
)

if /I "%build_ok%" NEQ "1" (
  echo "tests build failed"
  exit /B 1
)

echo "tests build ok"

del /Q .\tests\obj\*.obj