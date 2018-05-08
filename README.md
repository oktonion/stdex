# stdex
C++ 11 standart library implementation with extra features using C++98 only and POSIX threads. Minimum ugly preprocessor macro and no configuration files. 

Q: Why not boost?

A: 
1. Because I can.
2. Because I need something not so overheaded and ugly (ugly and beautiful actually, I really amazed by boost handling some features of C++).
3. Because I want all of basic little helpfull classes and tricks (as countof f.e.) be in one place with power of C++ 11 threads handling and template-programming.

Not all features of C++ 11 could be implemented without compiler support so I have no plans to include such features in the library.

The library is portable for any compiler that supports C++98 but as we all know there are lots of different implementations of std library in even major compilers so this library is tested on: 

* Borland C++ Builder 6.0 Updt4; 
* g++ 4.9, 4.8, 5.0 with c++98 option;
* clang 3.5, 5.0 with c++98 option;
* Visual Studio 2010, 2013, 2015;

...and thats it. If you use other compiler and decide to include my library in your project there is no guarantee that it will compile without errors. In case you will be able to fix the errors without breaking existing code for already supported compilers I would really appreciate your pull requests.

For the C++ threads (mutexes, threads) I'm using POSIX threads implementation. There is [one](https://github.com/GerHobbelt/pthread-win32 "I'm using this implementation") for Windows platform and definetely should be one for every POSIX-friendly platform.

The library is in development so no backward compability guaranteed with previous stdex. But one thing for sure: it will be more and more standart in the way of std library for C++ 11. 

# how to build
Build process is simple: either run a build_lib.sh script (works with gcc and clang if enviromental variable $COMPILER is set to compiler name, f.e. to 'clang++-3.5') or build by yourself static library from sources in 'stdex/src' directory.

# how to include in your project
In your project: 
* include sources of the library or link with prebuilded static library (.lib file, f.e. 'libstdex.lib')
* link with system libraries for POSIX-threads and realtime clocks: 'librt.lib' and 'libpthread.lib' in UNIX; 'ntdll.lib' and [POSIX-threads lib](https://github.com/GerHobbelt/pthread-win32 "I'm using this implementation") in Windows;
* enjoy

example script build for Ubuntu:
```
COMPILER=g++
$COMPILER main.cpp -L./stdex/lib/ -lstdex -lrt -lpthread -o "./bin/main"
```
