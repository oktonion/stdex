mkdir .\obj

setlocal enabledelayedexpansion

set "build_ok=1"
cd ..
set INCLUDE=%INCLUDE%;%cd%\pthread;

set build_opt="-Ox"

if ["%~1"]==["debug"] (
  set "build_opt="
  echo "debug build"
)

echo "compiling %VisualStudioVersion% pthread-win32"
cl -EHsc -W4 -Fo.\stdex\obj\pthread.obj -D HAVE_CONFIG_H -c ".\pthread-win32\pthread.c" %build_opt%
if ERRORLEVEL 1 set "build_ok=0"
if /I "%build_ok%" NEQ "1" (
    echo "failed"
    exit /B 1
)

cd .\stdex

for /f %%f in ('dir /b ".\src\*.cpp"') do (
  echo "compiling %%~nf"
  cl -EHsc -W4 -Fo.\obj\%%~nf.obj -c ".\src\%%f" %build_opt%
  if ERRORLEVEL 1 set "build_ok=0"
)

if /I "%build_ok%" NEQ "1" (
  echo "stdex.lib build failed"
  exit /B 1
)

mkdir .\lib

echo "archiving all obj-files to stdex.lib"
lib -out:".\lib\stdex.lib" ".\obj\*.obj"

dir .\lib\

echo "lib done"

del /Q .\obj\*.obj