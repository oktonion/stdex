# Change Log
All notable changes to this project will be documented in this file.

## [0.1.0] - 2018-08-13
### Removed
 - rudimental code for nullptr implementation
 
### Changed
  - is_array now detects T[] (array with no size)
  - nullptr now is signed as integer and enum (removes Borland C++ Builder 6.0 warning of signed-unsigned comparison)