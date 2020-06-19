mkdir .\tests\bin
mkdir .\tests\obj

setlocal enabledelayedexpansion
rem @ECHO OFF

set "build_ok=1"
set INCLUDE=%INCLUDE%;%cd%\pthread\;

set "tests_failed=failed tests:"

for /f %%f in ('dir /b ".\tests\*.cpp"') do (
  echo "compiling test %VisualStudioVersion% %%~nf"
  set "has_compile_error=0"
  set "has_compile_warn=0"
  cl -EHsc -W4 -Fo.\tests\obj\%%~nf.obj -D _CRT_SECURE_NO_WARNINGS -c ".\tests\%%f"
  @if ERRORLEVEL != 0 (
    set "has_compile_error=1"
  )
  call :CheckOutput %%~nf

  if %build_ok%==1 (
    set "has_compile_error=0"
    set "has_compile_warn=0"
    cl /I %cd%\pthread\ .\tests\obj\%%~nf.obj stdex.lib ntdll.lib -D _CRT_SECURE_NO_WARNINGS -Fe.\tests\bin\%%~nf.exe -link -LIBPATH:.\stdex\lib
    @if ERRORLEVEL != 0 (
      set "has_compile_error=1"
    )
    call :CheckOutput %%~nf
  )
)

if %build_ok% NEQ 1 (
  echo "%tests_failed%"
  exit /B 1
)

del /Q .\tests\obj\*.obj

:CheckOutput    - here starts the function

    if %has_compile_error%==1 (
      set "origin_str=%1"
      set "replaced_str=!origin_str:fail=!"
      if "!origin_str!" == "!replaced_str!" (
        set "build_ok=0"
        set "tests_failed=!tests_failed! %1"
      ) else (
        echo "failed as expected"
      )
    ) else (
      set "origin_str=%1"
      set "replaced_str=!origin_str:fail=_!"
      if not "!origin_str!" == "!replaced_str!" (
        set "build_ok=0"
        echo "not failed as expected"
        echo !MYOUTPUT!
        set "tests_failed=!tests_failed! %1"
      )
    )
goto:eof