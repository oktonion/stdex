mkdir .\tests\bin
mkdir .\tests\obj

setlocal enabledelayedexpansion

@ECHO OFF

set "build_ok=1"

set LF=^


REM The two empty lines are required here
set "MYOUTPUT="
set "tests_failed=failed tests:"

for /f %%f in ('dir /b ".\tests\*.cpp"') do (
  echo "compiling test Borland C++ %%~nf"
  set "MYOUTPUT="
  set "has_compile_error=0"
  set "has_compile_warn=0"
  for /f "delims=" %%i in ('bcc32 -X- -w-inl -w-ccc -tWC -Q -n.\tests\obj\ -L.\stdex\lib\ -I%cd%\pthread\ -c ".\tests\%%f"') do (
    if defined MYOUTPUT set "MYOUTPUT=!MYOUTPUT!!LF!"
    set "MYOUTPUT=!MYOUTPUT!%%i"
    set "origin_str=!MYOUTPUT!"
    set "replaced_str=!origin_str:Error E=!"
    if not "!origin_str!" == "!replaced_str!" (
      set "has_compile_error=1"
    )
    set "replaced_str=!origin_str:Warning W=!"
    if not "!origin_str!"=="!replaced_str!" (
      set "has_compile_warn=1"
    )
  )
  call :CheckOutput %%~nf

  if %build_ok%==1 (
    set "MYOUTPUT="
    set "has_compile_error=0"
    for /f %%i in ('bcc32 -X- -w-inl -w-ccc -tWC -Q -L.\stdex\lib\ -lap -I%cd%\pthread\ -e.\tests\bin\%%~nf.exe stdex.lib cw32mt.lib ntdll.lib .\tests\obj\%%~nf.obj') do (
      if defined MYOUTPUT set "MYOUTPUT=!MYOUTPUT!!LF!"
      set "MYOUTPUT=!MYOUTPUT!%%i"
      set "origin_str=!MYOUTPUT!"
      set "replaced_str=!origin_str:Error E=!"
      if not "!origin_str!" == "!replaced_str!" (
        set "has_compile_error=1"
      )
      set "replaced_str=!origin_str:Warning W=!"
      if not "!origin_str!"=="!replaced_str!" (
        set "has_compile_warn=1"
      )
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
        echo !MYOUTPUT!
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
      ) else (
        if %has_compile_warn%==1 echo !MYOUTPUT!
      )
    )
goto:eof