# Change Log
All notable changes to stdex library project will be documented in this file.

visit https://github.com/oktonion/stdex for the latest version of stdex library

## [0.1.1] - 2018-08-07
### Added
 - is_enum implementation

### Changed
 - is_class now returns false on enums

## [0.1.0] - 2018-08-03
### Removed
 - rudimental code for nullptr implementation
 
### Changed
 - is_array now detects T[] (array with no size)
 - nullptr now is signed as integer and enum (removes Borland C++ Builder 6.0 warning of signed-unsigned comparison)
  
 - build script for Microsoft Visual Studio now works correctly