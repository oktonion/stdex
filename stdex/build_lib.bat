mkdir .\obj

setlocal enabledelayedexpansion

set build_ok=1
cd ..
set INCLUDE=%INCLUDE%%cd%\pthread;

echo "compiling %VisualStudioVersion% pthread-win32"
cl -EHsc -Fo.\stdex\obj\pthread.obj -D HAVE_CONFIG_H -c ".\pthread-win32\pthread.c"
if /I "%ERRORLEVEL%" NEQ "0" (
    echo failed
    return 1
)

cd .\stdex

for /f %%f in ('dir /b ".\src\*.cpp"') do (
  echo "compiling %%~nf"
  cl -EHsc -Fo.\obj\%%~nf.obj -c ".\src\%%f"
  if /I "%ERRORLEVEL%" NEQ "0" (
    build_ok=0
  )
)

if /I "%build_ok%" NEQ "1" (
  echo "stdex.lib build failed"
  return 1
)

mkdir .\lib

echo "archiving all obj-files to stdex.lib"
lib -out:".\lib\stdex.lib" ".\obj\*.obj"

dir .\lib\

echo "lib done"

::del /Q .\obj\*.obj