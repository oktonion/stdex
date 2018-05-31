mkdir .\tests\bin
mkdir .\tests\obj

setlocal enabledelayedexpansion

set build_ok=1
set INCLUDE=%INCLUDE%%cd%\pthread\;

for /f %%f in ('dir /b ".\tests\*.cpp"') do (
  echo "compiling test %VisualStudioVersion% %%~nf"
  cl -EHsc -Fo.\tests\obj\%%~nf.obj -c ".\tests\%%f"
  if /I "%ERRORLEVEL%" NEQ "0" (
    %build_ok%=0
  )

  if /I "%build_ok%" NEQ "0" (
    cl .\tests\obj\%%~nf.obj stdex.lib "%WindowsSdkDir%Lib\ntdll.lib" -Fe.\tests\bin\%%~nf.exe -link -LIBPATH:.\stdex\lib
  )
)

if /I "%build_ok%" NEQ "1" (
  echo "tests build failed"
)

del /Q .\tests\obj\*.obj