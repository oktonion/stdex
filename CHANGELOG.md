# Change Log

All notable changes to stdex library project will be documented in this file.

visit [https://github.com/oktonion/stdex](https://github.com/oktonion/stdex) for the latest version of stdex library

## [0.2.11] - 2022-04-22

### Added

- polluting global namespace with `strerror_s` and `strerrorlen_s` symbols

### Removed

- stupid MS visual studio warnings

### Changed

- pthread implementation for Windows updated
- `stdex::error_category` string messages now thread-safe when possible (detecting and using thr-safe strerror_s if possible)

### Fixed

- `stdex::thread` handle now is initialized properly to default value

### Working on

- **futures** implementation
- **move semantics** emulation
- **function** implementation

## [0.2.10] - 2021-08-06

### Added

- GitHub actions as main CI pipeline for tests

### Removed

- extra headers includes

### Changed

- pthreads library for windows to latest version
- checks for `nullptr` be compatible with `NULL` provided by compiler

### Fixed

- some warnings for GCC
- comparison detection for int type nullptr emulation
- nullptr as enum implementation with valid sizeof(enum) == sizeof(void*)
- Visual Studio 2015 bug for `stdex::is_member_pointer` detection 
- `nullptr` feature detection

### Working on

- **futures** implementation
- **move semantics** emulation
- **function** implementation

## [0.2.9] - 2021-03-26

### Added

### Changed

- pthreads-win32 library with fresh fixes (affects Windows only)
- dynamic load of 'ntdll.dll' (no more need for 'ntdll.lib' linkage)
- when using `big_int` as internal implementation for `stdex::chrono::system_clock` and `stdex::chrono::steady_clock` duration counter define duration as `stdex::chrono::nanoseconds`

### Fixed

- static analysis warnings about uninitialized variables and negating unsigned values
- `stdex::condition_variable::wait_for`


## [0.2.8] - 2021-01-26

### Added

- `<mutex>` header:
- - `stdex::try_lock`
- - `stdex::lock`

- `<ctime>` header:
- - `stdex::timespec`
- - `STDEX_CLOCKS_PER_SEC` as `CLOCKS_PER_SEC`

- **big integer implementation** for `stdex::chrono`:
- - more precise clocks for compilers without 64 bit integers
- - more precise sleeps for all time-dependant functions and classes (like `stdex::timed_mutex` and such)
- - more precise ratio arithmetics for compilers without 64 bit integers

- moar tests to the god of tests

### Changed

- trying to be more clear why `std::terminate` has been raised (affects DEBUG mode only)
- `stdex::condition_variable` correct waitable functions
- `stdex::chrono::high_resolution_clock` defined in compile-time as most accurate clock from `stdex::chrono::system_clock` and `stdex::chrono::steady_clock`
- sync sleep clocks with `stdex::chrono` clocks
- `stdex::is_const` is now correcty detecting functions as non-const for bugged compilers
- `stdex::is_volatile` is now correcty detecting functions as non-volatile for bugged compilers
- `stdex::is_function` is now correcty detecting functions

### Fixed

- `<chrono>` header:
- - `stdex::chrono` clocks in general now are more precise and correct
- - `stdex::chrono::system_clock::is_steady` and `stdex::chrono::steady_clock::is_steady` flags are now correct for Windows
- - `stdex::chrono::system_clock` now converts to and from `time_t` properly
- - `stdex::chrono::duration` incorrect constructor fix

- `<system_error>` header:
- - `stdex::error_code` and `stdex::error_condition` have now proper comparison operators. That fix restrict compiler from using this comparison operators for user-provided types.
- - `stdex::is_error_code_enum` overload for `stdex::errc` is now correct
- - `stdex::is_enum` detection for integral types
- - `stdex::error_code` construction

- `<ctime>` header:
- - correct `swprintf` detection - fixing compiler specific implementations of `swprintf`

- qv-qualified array compiler bug

## [0.2.7] - 2020-07-14

### Added

- `namespace _stdex_ADL` for ADL hack for bugged compilers (pollutes global scope but gets job done)

- `<mutex>` header:
- - `stdex::timed_mutex` and `stdex::recursive_timed_mutex` implemented

- moar tests to the god of tests

### Changed

- `<mutex>` header:
- - throw exceptions if underlying pthread implementation is capable to detect errors (less bugs in production, more exceptions to handle, yeeeah!)

- all build and test run scripts now povide more clear output

### Fixed

- stupid MS defines for `max` and `min` breaking valid code

- `<thread>` header:
- - `stdex::this_thread::sleep_for` bug with sleep interval less than expected (sync OS clocks used in `<thread>` with OS clocks of `stdex::chrono::system_clock` and `stdex::chrono::steady_clock`)
- - bugged compilers problems with `stdex::thread` functor argument params deduction

- `<chrono>` implementation:
- - removed 1500ms time shift for MacOS implementation of `chrono`, this fix affects `stdex::chrono::system_clock::now()`, `stdex::chrono::steady_clock::now()`, `stdex::condition_variable::wait_for()`, all waitable mutexes etc.

- `<chrono>` header:
- - correct constructors set for `stdex::chrono::duration`
- - disabled `operator%` for floating point `stdex::chrono::duration::rep`
- - using correct trait `stdex::chrono::treat_as_floating_point` for detecting floating point `stdex::chrono::duration::rep`

- `<system_error>` header:
- - correct construction for custom error codes with `stdex::make_error_code` using ADL hack for bugged compilers
- - correct construction for custom error conditions with `stdex::make_error_condition` using ADL hack for bugged compilers

## [0.2.6] - 2020-06-17

### Added

- `<type_traits>` header: 
- - `stdex::is_function` trait detection for different calling conventions
- - `stdex::is_union` detection now includes more compiler intrinsics thus making it more accurate for distinguishing between *class* and *union*

- `<thread>` implementation:
- - `stdex::this_thread::sleep_for` is implemented as steady (monotonic) timer if available thus providing more precise sleep interval

### Changed

- improved threads blocking times

- `<chrono>` implementation:
- - `stdex::chrono::system_clock::is_steady` and `stdex::chrono::steady_clock::is_steady` are now correct
- -  using more precise and correct clocks if provided by OS (f.e. `CLOCK_MONOTONIC_RAW` for newer Linux) affects `stdex::chrono::system_clock::now()` and `stdex::chrono::steady_clock::now()`

### Fixed

- threads native handle is determined as invalid now for empty `stdex::thread` object

## [0.2.5] - 2020-04-08

### Changed

- includes now are consistent

### Fixed

- threads for 4+ args typo

## [0.2.4] - 2020-03-07

### Changed

- tabs to spaces in all header files
- C++ 98 standard support detection for GCC

### Fixed

- warnings for possible overflow in `<ratio>`
- warnings for using deprecated header `<strstream>`
- compiler bug that was leading to `std::out_of_range` exception in `stdex::stol`, `stdex::stoul` for perfectly valid u/long maximum, minimum value strings
- new lines at EOF for all '.hpp'

## [0.2.3] - 2019-12-09

### Added

- static asserts in constructors for `<system_error>` to forbid invalid constructor calls
- `<algorithm>` header:
- - SFINAE checks for `std::copy_n` existence

### Changed

- `<cstdint>` header:
- - hack for integral constant detection for Visual Studio 2019
- Borland Builder build script changed (removed extra cmd line arguments)

### Fixed

- `<string>` header:
- - removed warnings for signed/unsigned casts in `itol`, `itoul` etc.

## [0.2.2] - 2019-07-11

### Fixed

- `<typeindex>` header was missing so I found it in this release
- internals of chrono are now carefully hidden
- updated UNIX scripts to correct line endings

## [0.2.1] - 2019-07-03

### Added

- `<typeindex>` header:
- - `type_index` class
- - `type_info` defined in `stdex` namespace

### Changed

- clean internal macros

## [0.2.0] - 2019-07-01

### Added

- `to_string` and `to_wstring` now accept static arrays
- build script for Borland C++ Builder compiler

- `<cstdint>` header:
- - `intmax_t`, `uintmax_t`
- - `int8_t`, `uint8_t`
- - `int16_t`, `uint16_t`
- - `int32_t`, `uint32_t`
- - `int64_t`, `uint64_t`
- - `int_least8_t`, `uint_least8_t`
- - `int_least16_t`, `uint_least16_t`
- - `int_least32_t`, `uint_least32_t`
- - `int_least64_t`, `uint_least64_t`
- - `int_fast8_t`, `uint_fast8_t`
- - `int_fast16_t`, `uint_fast16_t`
- - `int_fast32_t`, `uint_fast32_t`
- - `int_fast64_t`, `uint_fast64_t`
- - `intptr_t`, `uintptr_t`

> All types are only in `stdex` namespace. Type is incomplete or absent if it is not supported.

- - STDEX_INTMAX_MIN, STDEX_INTMAX_MAX, STDEX_UINTMAX_MAX
- - STDEX_INT{*N*}_MIN, STDEX_INT{*N*}_MAX, STDEX_UINT{*N*}_MAX
- - STDEX_INT_LEAST{*N*}_MIN, STDEX_INT_LEAST{*N*}_MAX, STDEX_UINT_LEAST{*N*}MAX
- - STDEX_INT_FAST{*N*}_MIN, STDEX_INT_FAST{*N*}_MAX, STDEX_UINT_FAST{*N*}_MAX
- - STDEX_INTPTR_MIN, STDEX_INTPTR_MAX, STDEX_UINTPTR_MAX
- - STDEX_INTMAX_C(), STDEX_UINTMAX_C(), STDEX_INT{*N*}_C(), STDEX_UINT{*N*}_C()

> Where {*N*} is one in 8, 16, 32, 64 (if present). Defines are invalid or absent if type is not supported.

### Changed

- fixed bugs with `to_string` and `to_wstring` for (w)c-strings and (w)stringstream implementation
- "fixed" `is_union` implementation with nasty compiler-specific intrinsic (still returns `true` for classes if compiler has no intrinsic)
- changed pthread-win32 submodule dependency to my fork (origin have not been updated since forever)
- predefined types for `ratio` (like `yocto`, `milli`, `kilo`, `exa` etc.) are now incomplete if are not supported (too large or too small)
- all build scripts now work correctly
- fixed internal variables names with underscores

### Removed

- `<stdint_ex.h>` for `<cstdint.hpp>`/`<cstdint>`

## [0.1.3] - 2018-12-29

### Added

- `<iterator>` header:
- - `prev`
- - `next`
- - `begin`
- - `end`
- - all of C++ 98 std functions

- `<algorithm>` header:
- - `all_of`
- - `any_of`
- - `none_of`
- - `find_if_not`
- - `copy_if`
- - `copy_n`
- - `random_shuffle`
- - `is_partitioned`
- - `partition_copy`
- - `is_sorted_until`
- - `is_sorted`
- - `minmax`
- - `minmax_element`
- - `is_permutation`
- - all of C++ 98 std functions

### Changed

- fixed lots of micro bugs
- reimplemented `is_enum` and `is_union` (unions are still not distinguishable from classes)

## [0.1.2] - 2018-05-08

### Added

- `is_union` implementation with bug for detecting class as union
- Mac OS build added to CI (with xcode9 and clang-900.0.37)
- Mac OS implementation for `thread::hardware_concurrency` and `<chrono>`

## [0.1.1] - 2018-08-07

### Added

- `is_enum` implementation

### Changed

- `is_class` now returns `false` on enums

## [0.1.0] - 2018-08-03

### Removed

- rudimentary code for `nullptr` implementation

### Changed

- `is_array` now detects `T[]` (array with no size)
- `nullptr` now is signed as integer and enum (removes Borland C++ Builder 6.0 warning of signed-unsigned comparison)
  
- build script for Microsoft Visual Studio now works correctly
