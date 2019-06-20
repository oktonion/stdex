### CI Status

Branch   | Travis | Appveyor| Codecov
---------|--------|---------|---------
tests (development)    | [![Build Status](https://travis-ci.org/oktonion/stdex.svg?branch=tests)](https://travis-ci.org/oktonion/stdex) | [![Build Status](https://ci.appveyor.com/api/projects/status/hu8800gu31xldj25?svg=true)](https://ci.appveyor.com/project/oktonion/stdex) | [![codecov](https://codecov.io/gh/oktonion/stdex/branch/tests/graph/badge.svg)](https://codecov.io/gh/oktonion/stdex/branch/tests)
master   | [![Build Status](https://travis-ci.org/oktonion/stdex.svg?branch=master)](https://travis-ci.org/oktonion/stdex) | [![Build Status](https://ci.appveyor.com/api/projects/status/3c53qm34v1j37hy5/branch/master?svg=true)](https://ci.appveyor.com/project/oktonion/stdex-a309e/branch/master) | [![codecov](https://codecov.io/gh/oktonion/stdex/branch/master/graph/badge.svg)](https://codecov.io/gh/oktonion/stdex)


# stdex
C++ 11 standard library implementation with extra features using C++98 only and POSIX threads. Minimum ugly preprocessor macro and no configuration files. The library itself is built on top of C++ standard library distributed with your compiler and just extends it with C++11 features.

Q: Why not boost?

A: 
1. Because I can.
2. Because I need something without such overhead and ugliness (ugliness and beauty actually, I am really amazed by how Boost handles some features of C++).
3. Because I want all of basic little helpful classes and tricks (as countof f.e.) be in one place with power of C++ 11 threads handling and template-programming.

Not all features of C++ 11 can be implemented without compiler support so I have no plans to include such features in the library.

The library is portable for any compiler that supports C++98 but as we all know there are lots of different implementations of std library in even major compilers so this library is tested with:

* Borland C++ Builder 6.0 Updt4; 
* g++ 4.4.2, 4.4, 4.6;
* g++ 4.8, 4.9, 5.0 with c++98 option;
* clang 3.5, 5.0 with c++98 option;
* clang-900.0.37 with c++98 option (Mac OS);
* Visual Studio 2008, 2010, 2013, 2015;

...and thats it. If you use other compilers and decide to include this library in your project there is no guarantee that it will compile without errors. In case you will be able to fix the errors without breaking existing code for already supported compilers I would really appreciate your pull requests.

For the C++ threads (mutexes, threads) I'm using POSIX threads implementation. There is [one](https://github.com/GerHobbelt/pthread-win32 "I'm using this implementation") for Windows platform and definetely should be one for every POSIX-friendly platform.

The library is in development so no backward compability guaranteed with previous versions of stdex. But one thing for sure: it will be more and more standard in the way of std library for C++ 11.

# how to build
Build process is simple: 
* In Unix - either run a 'build_lib.sh' script (works with g++ and clang if environment variable $COMPILER is set to compiler name, f.e. to 'clang++-3.5') or build by yourself static library from 'stdex/src' directory sources.
* In Windows - either run a 'build_lib.bat' script (works with Visual Studio if environment variables are set by 'vsvars32.bat' script that is being shipped with your Visual Studio distributive) or build by yourself the static library from 'stdex/src' and 'pthreads-win32' directories sources.
* In QNX 6.x.x - either run a 'build_lib_qnx.sh' (using qcc compiler) or build by yourself the static library from 'stdex/src' directory sources (do not forget to link with 'stdlib' library).
* In Mac OS - either run a 'build_lib.sh' script (works with clang if environment variable $COMPILER is set to compiler name, f.e. to 'clang++-3.5') or build by yourself static library from 'stdex/src' directory sources.

# how to include in your project
In your project: 
1. include sources of the library or link with prebuilded static library ('.a'/'.lib' file, f.e. 'libstdex.a'/'stdex.lib')
2. link with system libraries for POSIX-threads and realtime clocks: 
* 'librt.lib' and 'libpthread.lib' in UNIX;
* 'ntdll.lib' and [POSIX-threads lib](https://github.com/GerHobbelt/pthread-win32 "I'm using this implementation") in Windows (if you have built stdex static library with 'pthreads-win32' sources then you have no need to link with pthreads anymore - it's already in 'stdex.lib');
* 'm.lib' in QNX 6.x.x;
* 'libpthread.lib' in Mac OS;
3. enjoy

example script build for Ubuntu (with g++ installed):
```
COMPILER=g++
$COMPILER main.cpp -L./stdex/lib/ -lstdex -lrt -lpthread -o "./bin/main"
```

example script build for Windows (with Visual Studio 2008 and Windows Kit 8.1 installed):
```
call "C:\Program Files (x86)\Microsoft Visual Studio 9.0\Common7\Tools\vsvars32.bat"
set LIB=%LIB%C:\Program Files (x86)\Windows Kits\8.1\Lib\winv6.3\um\x86\;
cl -EHsc -Fo.\obj\main.obj -c ".\main.cpp"
cl .\obj\main.obj stdex.lib ntdll.lib -Fe.\bin\main.exe -link -LIBPATH:.\stdex\lib
```

example script build for QNX 6.x.x (with qcc installed):
```
COMPILER=qcc
$COMPILER main.cpp -L./stdex/lib/ -lstdex -lm -o "./bin/main"
```

example script build for Mac OS (with clang installed):
```
COMPILER=clang++
$COMPILER main.cpp -L./stdex/lib/ -lstdex -lpthread -o "./bin/main"
```

visit https://github.com/oktonion/stdex for the latest version of stdex library
