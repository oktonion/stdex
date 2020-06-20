mkdir .\tests\bin
mkdir .\tests\obj

setlocal enabledelayedexpansion
@ECHO OFF

set true=1==1
set false=1==0

set build_ok=!true!

set INCLUDE=%INCLUDE%;%cd%\pthread\;

set "tests_failed=unsuccessful tests:"
set has_compile_error=!false!
set has_compile_warn=!false!

for /f %%f in ('dir /b ".\tests\*.cpp"') do (
  echo "compiling test Visual Studio C++ %VisualStudioVersion% %%~nf"
  set has_compile_error=!false!
  set has_compile_warn=!false!
  
  set "origin_str=%%~nf"
  set "replaced_str=!origin_str:fail=!"
  if "!origin_str!"=="!replaced_str!" (
    cl -nologo -EHsc -W4 -Fo.\tests\obj\%%~nf.obj -D _CRT_SECURE_NO_WARNINGS -c ".\tests\%%f"
  ) else (
    cl -nologo -EHsc -W4 -Fo.\tests\obj\%%~nf.obj -D _CRT_SECURE_NO_WARNINGS -c ".\tests\%%f" >nul 2>&1
  )
  if not !errorlevel!==0 (
    set has_compile_error=!true!
  )

  if !has_compile_error!==!true! (
    set "origin_str=%%~nf"
    set "replaced_str=!origin_str:fail=!"
    if "!origin_str!"=="!replaced_str!" (
      set build_ok=!false!
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
      set "tests_failed=!tests_failed! !origin_str!"
    )
  )
  
  if !has_compile_error!==!false! (
    cl -nologo /I %cd%\pthread\ .\tests\obj\%%~nf.obj stdex.lib ntdll.lib -D _CRT_SECURE_NO_WARNINGS -Fe.\tests\bin\%%~nf.exe -link -LIBPATH:.\stdex\lib
    if not !errorlevel!==0 (
      set has_compile_error=!true!
    )

    if !has_compile_error!==!true! (
      set "origin_str=%%~nf"
      set "replaced_str=!origin_str:fail=!"
      if "!origin_str!"=="!replaced_str!" (
        set build_ok=!false!
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
        set "tests_failed=!tests_failed! !origin_str!"
      )
    )
  )
)

if !build_ok!==!false! (
  echo !tests_failed!
  exit /B 1
)

del /Q .\tests\obj\*.obj