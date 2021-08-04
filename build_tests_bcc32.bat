mkdir .\tests\bin
mkdir .\tests\obj

setlocal enabledelayedexpansion

@ECHO OFF

set true=1==1
set false=1==0

set build_ok=!true!

REM The two empty lines are required here
set "tests_failed=unsuccessful tests:"
set has_compile_error=!false!
set has_compile_warn=!false!
set current_test_is_ok=!false!
set line_n=0

time /T

for /f  %%f in ('dir /b ".\tests\*.cpp"') do (
  echo "compiling test Borland C++ %%~nf"
  set has_compile_error=!false!
  set has_compile_warn=!false!
  set /A line_n=0
  for /f "delims=" %%i in ('bcc32 -X- -w-inl -w-ccc -tWC -Q -n.\tests\obj\ -L.\stdex\lib\ -I%cd%\pthread\ -c ".\tests\%%f"') do (
    set "MYOUTPUT=%%i"
    set "origin_str=!MYOUTPUT!"
    set "replaced_str=!origin_str:Error E=!"
    if not "!origin_str!"=="!replaced_str!" (
      set has_compile_error=!true!
    )
    set "replaced_str=!origin_str:Error:=!"
    if not "!origin_str!"=="!replaced_str!" (
      set has_compile_error=!true!
    )
    set "replaced_str=!origin_str:Assertion failed=!"
    if not "!origin_str!"=="!replaced_str!" (
      set has_compile_error=!true!
    )
    set "replaced_str=!origin_str:Fatal:=!"
    if not "!origin_str!"=="!replaced_str!" (
      set has_compile_error=!true!
    )
    set "replaced_str=!origin_str:Internal compiler error=!"
    if not "!origin_str!"=="!replaced_str!" (
      set has_compile_error=!true!
    )
    set "replaced_str=!origin_str:Warning W=!"
    if not "!origin_str!"=="!replaced_str!" (
      set has_compile_warn=!true!
    )
    set "replaced_str=!origin_str:Warning:=!"
    if not "!origin_str!"=="!replaced_str!" (
      set has_compile_warn=!true!
    )
    set "ouput_line!line_n!=%%i"
    set /A line_n=!line_n!+1
  )
  
  set current_test_is_ok=!true!

  if !has_compile_error!==!true! (
    set "origin_str=%%~nf"
    set "replaced_str=!origin_str:fail=!"
    if "!origin_str!"=="!replaced_str!" (
      set build_ok=!false!
      set current_test_is_ok=!false!
      for /l %%i in (0, 1, !line_n!) do (
        if defined ouput_line%%i echo !ouput_line%%i!
      )
      set "tests_failed=!tests_failed! !origin_str!"
    ) else (
      echo "failed as expected"
    )
  ) else (
    set "origin_str=%%~nf"
    set "replaced_str=!origin_str:fail=_!"
    if not "!origin_str!"=="!replaced_str!" (
      set build_ok=!false!
      set current_test_is_ok=!false!
      echo "not failed as expected"
      for /l %%i in (0, 1, !line_n!) do (
        if defined !ouput_line%%i! echo !ouput_line%%i!
      )
      set "tests_failed=!tests_failed! !origin_str!"
    ) else (
      if !has_compile_warn!==!true! (
        for /l %%i in (0, 1, !line_n!) do (
          if defined ouput_line%%i echo !ouput_line%%i!
        )
      )
    )
  )

  set line_n=0
  
  if !current_test_is_ok!==!true! if !has_compile_error!==!false! (
    set /A line_n=0
    for /f "delims=" %%i in ('bcc32 -X- -w-inl -w-ccc -tWC -Q -L.\stdex\lib\ -lap -I%cd%\pthread\ -e.\tests\bin\%%~nf.exe stdex.lib cw32mt.lib .\tests\obj\%%~nf.obj') do (
      set "MYOUTPUT=!MYOUTPUT!%%i"
      set "origin_str=!MYOUTPUT!"
      set "replaced_str=!origin_str:Error E=!"
      if not "!origin_str!"=="!replaced_str!" (
        set has_compile_error=!true!
      )
      set "replaced_str=!origin_str:Error:=!"
      if not "!origin_str!"=="!replaced_str!" (
        set has_compile_error=!true!
      )
      set "replaced_str=!origin_str:Fatal:=!"
      if not "!origin_str!"=="!replaced_str!" (
        set has_compile_error=!true!
      )
      set "replaced_str=!origin_str:Internal compiler error=!"
      if not "!origin_str!"=="!replaced_str!" (
        set has_compile_error=!true!
      )
      set "replaced_str=!origin_str:Warning W=!"
      if not "!origin_str!"=="!replaced_str!" (
        set has_compile_warn=!true!
      )
      set "replaced_str=!origin_str:Warning:=!"
      if not "!origin_str!"=="!replaced_str!" (
        set has_compile_warn=!true!
      )
      set "ouput_line!line_n!=%%i"
      set /A line_n=!line_n!+1
    )
    
    if !has_compile_error!==!true! (
      set "origin_str=%%~nf"
      set "replaced_str=!origin_str:fail=!"
      if "!origin_str!"=="!replaced_str!" (
        set build_ok=!false!
        for /l %%i in (0, 1, !line_n!) do (
          if defined ouput_line%%i echo !ouput_line%%i!
        )
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
        for /l %%i in (0, 1, !line_n!) do (
          if defined ouput_line%%i echo !ouput_line%%i!
        )
        set "tests_failed=!tests_failed! !origin_str!"
      ) else (
        if !has_compile_warn!==!true! (
          for /l %%i in (0, 1, !line_n!) do (
            if defined ouput_line%%i echo !ouput_line%%i!
          )
        )
      )
    )
  )
)

if !build_ok!==!false! (
  echo !tests_failed!
  exit /B 1
)

time /T

del /Q .\tests\obj\*.obj