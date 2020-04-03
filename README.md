### CI Status and tests coverage

Branch   | Travis | Appveyor| Codecov
---------|--------|---------|---------
tests (development)    | [![Build Status](https://travis-ci.org/oktonion/stdex.svg?branch=tests)](https://travis-ci.org/oktonion/stdex) | [![Build Status](https://ci.appveyor.com/api/projects/status/hu8800gu31xldj25?svg=true)](https://ci.appveyor.com/project/oktonion/stdex) | [![codecov](https://codecov.io/gh/oktonion/stdex/branch/tests/graph/badge.svg)](https://codecov.io/gh/oktonion/stdex/branch/tests)
master   | [![Build Status](https://travis-ci.org/oktonion/stdex.svg?branch=master)](https://travis-ci.org/oktonion/stdex) | [![Build Status](https://ci.appveyor.com/api/projects/status/3c53qm34v1j37hy5/branch/master?svg=true)](https://ci.appveyor.com/project/oktonion/stdex-a309e/branch/master) | [![codecov](https://codecov.io/gh/oktonion/stdex/branch/tests/graph/badge.svg)](https://codecov.io/gh/oktonion/stdex)

[![Language grade: C/C++](https://img.shields.io/lgtm/grade/cpp/g/oktonion/stdex.svg?logo=lgtm&logoWidth=18)](https://lgtm.com/projects/g/oktonion/stdex/context:cpp) | [![Codacy Badge](https://api.codacy.com/project/badge/Grade/b74b5f48d04641de81c8c76e3fc26f90)](https://www.codacy.com/app/oktonion/stdex?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=oktonion/stdex&amp;utm_campaign=Badge_Grade)

# stdex

C++ 11 standard library implementation with extra features using C++98 only and POSIX threads. Minimum ugly preprocessor macro and no configuration files. The library itself is built on top of C++ standard library distributed with your compiler and just extends it with C++11 features.

## Supported platforms

* Windows (from XP to 10)
* Unix (any with `glibc` and `pthread`)
* MacOS (any)
* QNX 6.x.x
* QNX 4 (partly)

[Wiki](https://github.com/oktonion/stdex/wiki) for more details.

Q: Why not [Boost](https://github.com/boostorg)?

A:

1. Because I can.
2. Because I need something without such overhead and ugliness (ugliness and beauty actually, I am really amazed by how Boost handles some features of C++).
3. Because I want all of basic little helpful classes and tricks (as `countof` f.e.) be in one place with power of C++ 11 threads handling and template-programming.

<details>
<summary><b>C++ 11 implemented features list</b></summary>

### Language core

* **nullptr** and `stdex::nullptr_t`
* **static_assert** or `STATIC_ASSERT` macro
* **stdex_char16_t** and **stdex_char32_t** (as equivalent to **char16_t**, **char32_t**)
* **stdex_noexcept** (as partial equivalent to **noexcept**)
* `stdex_assert` macro (as equivalent to `cassert` macro)

### Utilities library

| name                 | purpose                                                                        |  implemented | is final |
|----------------------|--------------------------------------------------------------------------------|:------------:|----------|
| `<typeindex>`        | stdex::type_index                                                              | 100%         | maybe    |
| `<type_traits>`      | Compile-time type information                                                  | 70%          | no       |
| `<functional>`       | Function objects, Function invocations, Bind operations and Reference wrappers | no           | no       |
| `<chrono>`           | C++ time utilities                                                             | 100%         | yes      |
| `<initializer_list>` | stdex::initializer_list class template                                         | no and never | yes      |
| `<tuple>`            | stdex::tuple class template                                                    | no           | no       |

### Dynamic memory management

| name                 | purpose                |  implemented | is final |
|----------------------|------------------------|:------------:|----------|
| `<scoped_allocator>` | Nested allocator class | no           | maybe    |

### Numeric limits

| name                 | purpose                                                        |  implemented | is final |
|----------------------|----------------------------------------------------------------|:------------:|----------|
| `<cstdint>`          | fixed-size types and limits of other types                     | 99%          | maybe    |
| `<cinttypes>`        | formatting macros, intmax_t and uintmax_t math and conversions | no           | maybe    |

### Error handling

| name                 | purpose                                                    |  implemented | is final |
|----------------------|------------------------------------------------------------|:------------:|----------|
| `<system_error>`     | defines stdex::error_code, a platform-dependent error code | 100%         | yes      |

### Strings library

| name                 | purpose                                                 |  implemented | is final |
|----------------------|---------------------------------------------------------|:------------:|----------|
| `<cuchar>`           | C-style  Unicode character conversion functions         | no           | maybe    |
| `<string>`           | stdex::basic_string, stdex::stoi, stdex::to_string etc. | 50%          | no       |

### Containers library

| name              | purpose                                | implemented | is final |
|-------------------|----------------------------------------|:-----------:|----------|
| `<array>`         | stdex::array container                 | no          | maybe    |
| `<forward_list>`  | stdex::forward_list container          | no          | maybe    |
| `<unordered_set>` | unordered associative `set` containers | no          | maybe    |
| `<unordered_map>` | unordered associative `map` containers | no          | maybe    |

### Iterators library

| name              | purpose         | implemented | is final |
|-------------------|-----------------|:-----------:|----------|
| `<iterator>`      | Range iterators | 100%        | yes      |

### Algorithms library

| name              | purpose                           | implemented | is final |
|-------------------|-----------------------------------|:-----------:|----------|
| `<algorithm>`     | Algorithms that operate on ranges | 100%        | yes      |

### Numerics library

| name              | purpose                                     | implemented | is final |
|-------------------|---------------------------------------------|:-----------:|----------|
| `<random>`        | Random number generators and distributions  | no          | maybe    |
| `<ratio>`         | Compile-time rational arithmetic            | 99%         | maybe    |
| `<cfenv>`         | Floating-point environment access functions | no          | maybe    |

### Input/output library

| name              | purpose                                                                                                                | implemented | is final |
|-------------------|------------------------------------------------------------------------------------------------------------------------|:-----------:|----------|
| `<sstream>`       | stdex::basic_stringstream, stdex::basic_istringstream, stdex::basic_ostringstream class templates and several typedefs | 30%         | maybe    |

### Regular Expressions library

| name              | purpose                                                                    | implemented | is final |
|-------------------|----------------------------------------------------------------------------|:-----------:|----------|
| `<regex>`         | Classes, algorithms and iterators to support regular expression processing | no          | maybe    |

### Atomic Operations library

| name              | purpose                   | implemented | is final |
|-------------------|---------------------------|:-----------:|----------|
| `<atomic>`        | Atomic operations library | no          | no       |

### Thread support library

| name                   | purpose                                       | implemented | is final |
|------------------------|-----------------------------------------------|:-----------:|----------|
| `<thread>`             | stdex::thread class and  supporting functions | 90%         | no       |
| `<mutex>`              | mutual exclusion primitives                   | 100%        | yes      |
| `<future>`             | primitives for asynchronous computations      | no          | maybe    |
| `<condition_variable>` | thread waiting conditions                     | 100%        | yes      |

</details>

Not all features of C++ 11 can be implemented without compiler support so I have no plans to include such features in the library.

The library is portable for any compiler that supports C++98 but as we all know there are lots of different implementations of std library in even major compilers so this library is tested with:

* `Borland C++ Builder 6.0` Updt4;
* `qcc` (just a wrapper over `gcc` in QNX)
* `g++` 3.4, 4.4.2, 4.4, 4.6;
* `g++` 4.8, 4.9, 5.0 with c++98 option;
* `clang` 3.5, 5.0 with c++98 option;
* `clang-900.0.37` with c++98 option (Mac OS);
* `Visual Studio` 2008, 2010, 2013, 2015, 2017;

...and that's it. If you use other compilers and decide to include this library in your project there is no guarantee that it will compile without errors. In case you will be able to fix the errors without breaking existing code for already supported compilers I would really appreciate your pull requests.

For the C++ threads (mutexes, threads) I'm using POSIX threads implementation. There is [one](https://github.com/oktonion/pthread-win32 "I'm using this implementation") for Windows platform and definitely should be one for every POSIX-friendly platform.

The library is in development so no backward compatibility guaranteed with previous versions of **stdex**. But one thing for sure: it will be more and more standard in the way of std library for C++ 11.

# how to build

Build process is simple:

* For Unix - either run a `build_lib.sh` script (works with `g++` and `clang` if environment variable `$COMPILER` is set to compiler name, f.e. to `clang++-3.5`) or build by yourself static library from `stdex/src` directory sources.
* For Windows
* * run a `build_lib.bat` script that works with Visual Studio's `cl` if environment variables are set by `v{s/c}vars{32/64/all}.bat` (`vcvars32.bat`, `vsvars32.bat` etc.) script that is in your Visual Studio distributive;
* * run a `build_lib_bcc32.bat` script for Borland C++ Builder 6.0 Updt4 `bcc32` build;
* * ...or build by yourself the static library from `stdex/src` and `pthreads-win32` directories sources.
* For QNX 6.x.x - either run a `build_lib_qnx.sh` (using `qcc` compiler) or build by yourself the static library from `stdex/src` directory sources (do not forget to link with `stdlib` library).
* For Mac OS - either run a `build_lib.sh` script (works with clang if environment variable `$COMPILER` is set to compiler name, f.e. to `clang++-3.5`) or build by yourself static library from `stdex/src` directory sources.

# how to include in your project

In your project:

1. include sources of the library or link with prebuild static library (`.a`/`.lib` file, f.e. `libstdex.a`/`stdex.lib`)
2. link with system libraries for POSIX-threads and real-time clocks:

    * `librt.lib` and `libpthread.lib` for UNIX;
    * `ntdll.lib` and [POSIX-threads lib](https://github.com/oktonion/pthread-win32 "I'm using this implementation") for Windows (if you have built stdex static library with `pthreads-win32` sources then you have no need to link with pthreads anymore - it's already in `stdex.lib`);
    > Note: `ntdll.lib` is not present in Borland C++ Builder 6.0 Updt4 so you should make one with `coff2omf.exe` utility (located in "Borland\CBuilder6\Bin" folder) from Windows SDK/Windows Kit version of `ntdll.lib`.
    * `m.lib` for QNX 6.x.x;
    * `libpthread.lib` for Mac OS;

3. enjoy

example script build for Ubuntu (with `g++` installed):

```sh
COMPILER=g++
$COMPILER main.cpp -L./stdex/lib/ -lstdex -lrt -lpthread -o "./bin/main"
```

example script build for Windows (with Visual Studio 2008 and Windows Kit 8.1 installed):

```bat
call "C:\Program Files (x86)\Microsoft Visual Studio 9.0\Common7\Tools\vsvars32.bat"
set LIB=%LIB%C:\Program Files (x86)\Windows Kits\8.1\Lib\winv6.3\um\x86\;
set INCLUDE=%INCLUDE%;%cd%\pthread;
cl -EHsc -Fo.\obj\main.obj -c ".\main.cpp"
cl .\obj\main.obj stdex.lib ntdll.lib -Fe.\bin\main.exe -link -LIBPATH:.\stdex\lib
```

example script build for Windows (with Borland C++ Builder 6.0 Updt4 and generated `ntdll.lib` with `coff2omf.exe`):

```bat
bcc32 -w-inl -w-ccc -tWM -Q -n.\obj\ -I%cd%\pthread\ -c ".\main.cpp"
bcc32 -w-inl -w-ccc -tWM -Q -L.\stdex\lib\ -lap -I%cd%\pthread\ -e.\bin\main.exe stdex.lib cw32mt.lib ntdll.lib .\obj\main.obj
```

example script build for QNX 6.x.x (with `qcc` installed):

```sh
COMPILER=qcc
$COMPILER main.cpp -L./stdex/lib/ -lstdex -lm -o "./bin/main"
```

example script build for Mac OS (with `clang` installed):

```sh
COMPILER=clang++
$COMPILER main.cpp -L./stdex/lib/ -lstdex -lpthread -o "./bin/main"
```

visit [https://github.com/oktonion/stdex](https://github.com/oktonion/stdex) for the latest version of stdex library
