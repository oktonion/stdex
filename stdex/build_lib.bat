mkdir .\obj

setlocal enabledelayedexpansion

set build_ok=1
set filename=""
cd ..
set INCLUDE=%INCLUDE%%cd%\pthread-win32\;
cd ./stdex

for /f %%f in ('dir /b ".\src\*.cpp"') do (
  echo "compiling %%~nf"
  cl -EHsc -Fo.\obj\%%~nf.o -c ".\src\%%f"
  if /I "%ERRORLEVEL%" NEQ "0" (
    build_ok=0
  )
)

  if /I "%build_ok%" NEQ "1" (
    echo "stdex.lib build failed"
    return 1
  )

  mkdir .\lib

  for /f %%f in ('dir /b ".\obj\*.o"') do (
  echo %%f
  echo "archiving %%~nf to stdex.lib"
  lib -out:".\lib\stdex.lib" ".\obj\%%f"
)

dir .\lib\

echo "lib done"