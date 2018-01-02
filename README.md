# stdex
C++ 11 standart library implementation with extra features using C++98 only and POSIX threads. Minimum ugly preprocessor macro and no configuration files. 

Q: Why not boost?

A: 
1. Because I can.
2. Because I need something not so overheaded and ugly (ugly and beautiful actually, I really amazed by boost handling some features of C++).
3. Because I want all of basic little helpfull classes and tricks (as countof f.e.) be in one place with power of C++ 11 threads handling and template-programming.

Not all features of C++ 11 could be implemented without compiler support so I have no plans to include such features in the library.

The library is portable for any compiler that supports C++98 but as we all know there are lots of different implementations of std library in even major compilers so this library is tested on C++ Builder 6.0 Updt4, g++ 4.something, Visual Studio 2013, 2015 and thats it. If you use other compiler and decide to include my library in your project there is no guarantee that it will compile without errors. In case you will be able to fix the errors without breaking existing code for already supported compilers I would really appreciate your pull requests.

For the C++ threads (mutexes, threads) I'm using POSIX threads implementation. There is one for Windows platform and definetely should be one for every POSIX-friendly platform.

The library is in development so no backward compability guaranteed with previous stdex. But one thing for sure: it will be more and more standart in the way of std library for C++ 11. 
