mkdir .\tests\bin
mkdir .\tests\obj

setlocal enabledelayedexpansion
@echo off

set "build_ok=1"
set INCLUDE=%INCLUDE%;%cd%\pthread\;

for /f %%f in ('dir /b ".\tests\*.cpp"') do (
  echo "compiling test %VisualStudioVersion% %%~nf"
  cl -EHsc -W4 -Fo.\tests\obj\%%~nf.obj -D _CRT_SECURE_NO_WARNINGS -c ".\tests\%%f"
  set origin_str=%%f
  set replaced_str=!origin_str:fail=_!
  IF ERRORLEVEL 1 (
    if not "%origin_str%"=="%replaced_str%" set "build_ok=0"
  )

  if "%build_ok%"=="1" (
    cl /I %cd%\pthread\ .\tests\obj\%%~nf.obj stdex.lib ntdll.lib -D _CRT_SECURE_NO_WARNINGS -Fe.\tests\bin\%%~nf.exe -link -LIBPATH:.\stdex\lib
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