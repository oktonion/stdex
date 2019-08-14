mkdir .\obj

setlocal enabledelayedexpansion

set "build_ok=1"
cd ..

set build_opt="-O2"

if ["%~1"]==["debug"] (
  set "build_opt="
  echo "debug build"
)

echo "compiling Borland C++ pthread-win32"
bcc32 -X- -w -tWC -Q -n.\stdex\obj\ -DHAVE_CONFIG_H;PTW32_STATIC_LIB -I%cd%\pthread\ -c .\pthread-win32\pthread.c %build_opt%
if ERRORLEVEL 1 set "build_ok=0"
if /I "%build_ok%" NEQ "1" (
    echo "failed"
    exit /B 1
)

cd .\stdex

for /f %%f in ('dir /b ".\src\*.cpp"') do (
  echo "compiling %%~nf"
  bcc32 -X- -w -tWC -Q -n.\obj\ -I%cd%\..\pthread\ -c ".\src\%%f" %build_opt%
  if ERRORLEVEL 1 (
    set "build_ok=0"
  )
)

if /I "%build_ok%" NEQ "1" (
  echo "stdex.lib build failed"
  exit /B 1
)

cd .\obj

echo "archiving all obj-files to stdex.lib"
for /f %%f in ('dir /b ".\*.obj"') do (
    echo "add %%~nf to stdex.lib"
    tlib stdex /a %%~nf
)

cd ..
mkdir .\lib

move .\obj\stdex.lib .\lib\stdex.lib

dir .\lib\

echo "lib done"

del /Q .\obj\*.obj