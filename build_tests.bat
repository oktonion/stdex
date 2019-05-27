mkdir .\tests\bin
mkdir .\tests\obj

setlocal enabledelayedexpansion

set "build_ok=true"
set INCLUDE=%INCLUDE%;%cd%\pthread\;

for /f %%f in ('dir /b ".\tests\*.cpp"') do (
  echo "compiling test %VisualStudioVersion% %%~nf"
  cl -EHsc -W4 -Fo.\tests\obj\%%~nf.obj -c ".\tests\%%f"
  IF ERRORLEVEL 1 (
    set "build_ok=false"
  )

  if /I "%build_ok%" NEQ "0" (
    cl /I %cd%\pthread\ .\tests\obj\%%~nf.obj stdex.lib ntdll.lib -Fe.\tests\bin\%%~nf.exe -link -LIBPATH:.\stdex\lib
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