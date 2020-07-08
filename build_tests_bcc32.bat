mkdir .\tests\bin
mkdir .\tests\obj

setlocal enabledelayedexpansion

@ECHO OFF

set true=1==1
set false=1==0

set build_ok=!true!

set LF=^


REM The two empty lines are required here
set "MYOUTPUT="
set "MYOUTPUT2="
set "MYOUTPUT3="
set "tests_failed=unsuccessful tests:"
set has_compile_error=!false!
set has_compile_warn=!false!


for /f %%f in ('dir /b ".\tests\*.cpp"') do (
  echo "compiling test Borland C++ %%~nf"
  set "MYOUTPUT="
  set "MYOUTPUT2="
  set "MYOUTPUT3="
  set has_compile_error=!false!
  set has_compile_warn=!false!
  set /A line_n=0
  for /f "delims=" %%i in ('bcc32 -X- -w-inl -w-ccc -tWC -Q -n.\tests\obj\ -L.\stdex\lib\ -I%cd%\pthread\ -c ".\tests\%%f"') do (
    if defined MYOUTPUT set "MYOUTPUT=!MYOUTPUT!!LF!"
    set "MYOUTPUT=!MYOUTPUT!%%i"
    set "origin_str=!MYOUTPUT!"
    set "replaced_str=!origin_str:Error E=!"
    if not "!origin_str!"=="!replaced_str!" (
      set has_compile_error=!true!
    )
    set "replaced_str=!origin_str:Warning W=!"
    if not "!origin_str!"=="!replaced_str!" (
      set has_compile_warn=!true!
    )
    set /A line_n=!line_n!+1
    if !line_n! GEQ 50 (
      if !line_n! GEQ 100 (
        if "!MYOUTPUT2!"=="" (
          set "MYOUTPUT2=!MYOUTPUT!"
          set "MYOUTPUT="
        )
      ) else (
        if "!MYOUTPUT3!"=="" (
          set "MYOUTPUT3=!MYOUTPUT!"
          set "MYOUTPUT="
        )
      )
    )
  )
  
  if !has_compile_error!==!true! (
    set "origin_str=%%~nf"
    set "replaced_str=!origin_str:fail=!"
    if "!origin_str!"=="!replaced_str!" (
      set build_ok=!false!
      echo !MYOUTPUT3!
      echo !MYOUTPUT2!
      echo !MYOUTPUT!
      set "tests_failed=!tests_failed! !origin_str!"
    ) else (
      echo "failed as expected"
    )
  ) else (
    set "origin_str=%%~nf"
    set "replaced_str=!origin_str:fail=_!"
    if not "!origin_str!"=="!replaced_str!" (
      set build_ok=!false!
      echo "not failed as expected"
      echo !MYOUTPUT3!
      echo !MYOUTPUT2!
      echo !MYOUTPUT!
      set "tests_failed=!tests_failed! !origin_str!"
    ) else (
      if !has_compile_warn!==!true! echo !MYOUTPUT!
    )
  )
  
  if !has_compile_error!==!false! (
    set "MYOUTPUT="
    set "MYOUTPUT2="
    set "MYOUTPUT3="
    set /A line_n=0
    for /f %%i in ('bcc32 -X- -w-inl -w-ccc -tWC -Q -L.\stdex\lib\ -lap -I%cd%\pthread\ -e.\tests\bin\%%~nf.exe stdex.lib cw32mt.lib ntdll.lib .\tests\obj\%%~nf.obj') do (
      if defined MYOUTPUT set "MYOUTPUT=!MYOUTPUT!!LF!"
      set "MYOUTPUT=!MYOUTPUT!%%i"
      set "origin_str=!MYOUTPUT!"
      set "replaced_str=!origin_str:Error E=!"
      if not "!origin_str!"=="!replaced_str!" (
        set has_compile_error=!true!
      )
      set "replaced_str=!origin_str:Warning W=!"
      if not "!origin_str!"=="!replaced_str!" (
        set has_compile_warn=!true!
      )
      set /A line_n=!line_n!+1
      if !line_n! GEQ 50 (
        if !line_n! GEQ 100 (
          set "MYOUTPUT2=!MYOUTPUT!"
          set "MYOUTPUT="
        ) else (
          set "MYOUTPUT3=!MYOUTPUT!"
          set "MYOUTPUT="
        )
      )
    )
    
    if !has_compile_error!==!true! (
      set "origin_str=%%~nf"
      set "replaced_str=!origin_str:fail=!"
      if "!origin_str!"=="!replaced_str!" (
        set build_ok=!false!
        echo !MYOUTPUT3!
        echo !MYOUTPUT2!
        echo !MYOUTPUT!
        set "tests_failed=!tests_failed! !origin_str!"
      ) else (
        echo "failed as expected"
      )
    ) else (
      set "origin_str=%%~nf"
      set "replaced_str=!origin_str:fail=_!"
      if not "!origin_str!"=="!replaced_str!" (
        set build_ok=!false!
        echo "not failed as expected"
        echo !MYOUTPUT3!
        echo !MYOUTPUT2!
        echo !MYOUTPUT!
        set "tests_failed=!tests_failed! !origin_str!"
      ) else (
        if !has_compile_warn!==!true! (
          echo !MYOUTPUT3!
          echo !MYOUTPUT2!
          echo !MYOUTPUT!
        )
      )
    )
  )
)

if !build_ok!==!false! (
  echo !tests_failed!
  exit /B 1
)

rem del /Q .\tests\obj\*.obj