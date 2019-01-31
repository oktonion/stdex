# Change Log

All notable changes to stdex library project will be documented in this file.

visit https://github.com/oktonion/stdex for the latest version of stdex library

## [0.1.4] - 2019-xx-xx

### Changed

- fixed bugs with `to_string` and `to_wstring` for (w)c-strings and (w)stringstream implementation

## [0.1.3] - 2018-12-29

### Added

- iterator header
- - prev
- - next
- - begin
- - end
- - all of C++ 98 std functions
- algorithm header
- - all_of
- - any_of
- - none_of
- - find_if_not
- - copy_if
- - copy_n
- - random_shuffle
- - is_partitioned
- - partition_copy
- - is_sorted_until
- - is_sorted
- - minmax
- - minmax_element
- - is_permutation
- - all of C++ 98 std functions

### Changed

- fixed lots of micro bugs
- reimplemented is_enum and is_union (unions are still not distinguishable from classes)

## [0.1.2] - 2018-05-08

### Added

- is_union implementation with bug for detecting class as union
- Mac OS build added to CI (with xcode9 and clang-900.0.37)
- Mac OS implementation for thread::hardware_concurrency and chrono

## [0.1.1] - 2018-08-07

### Added

- is_enum implementation

### Changed

- is_class now returns false on enums

## [0.1.0] - 2018-08-03

### Removed

- rudimentary code for nullptr implementation
 
### Changed

- is_array now detects T[] (array with no size)
- nullptr now is signed as integer and enum (removes Borland C++ Builder 6.0 warning of signed-unsigned comparison)
  
- build script for Microsoft Visual Studio now works correctly