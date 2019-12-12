# Change Log

All notable changes to stdex library project will be documented in this file.

visit [https://github.com/oktonion/stdex](https://github.com/oktonion/stdex) for the latest version of stdex library

## [0.2.4] - 2020-xx-xx

### Added

- some serious changes

### Changed

- tabs to spaces in all header files
- C++ 98 standard support detection for GCC

### Fixed

- warnings for possible overflow in `<ratio>`
- warnings for using deprecated header `<strstream>`

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
