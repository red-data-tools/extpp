# News

## 0.1.1 - 2022-11-02

### Improvements

  * Removed needless a workaround for old Ruby with C++17.

## 0.1.0 - 2021-10-03

### Improvements

  * Implemented as header only library.

## 0.0.9 - 2020-11-03

### Improvements

  * Improved `clang++` version detection.
    [GitHub#10][Patch by Alex Neill]

  * Improved `g++` version detection.

### Thanks

  * Alex Neill

## 0.0.8 - 2019-09-25

### Improvements

  * Changed to use `RbConfig::CONFIG["SOEXT"]`.
    [Suggested by Nobuyoshi Nakada]

  * Added `ExtpPP::Platform#shared_library_extension`.

  * Added support for macOS Majave.
    [GitHub#8][Patch by Josh Huckabee]

### Thanks

  * Nobuyoshi Nakada

  * Josh Huckabee

## 0.0.7 - 2019-03-21

### Improvements

  * Added support for Windows.

## 0.0.6 - 2019-02-25

### Improvements

  * Suppressed warnings by `register`.
    [GitHub#6][Patch by Kenta Murata]

  * Added `noreturn` attribute to suppress warning.
    [GitHub#7][Patch by Kenta Murata]

### Thanks

  * Kenta Murata

## 0.0.5 - 2019-02-25

### Improvements

  * Added support for Xcode 10.
    [GitHub#4][Reported by Kenta Murata]

### Thanks

  * Kenta Murata

## 0.0.4 - 2019-02-16

### Improvements

  * `rb::Class(const char *name)`: Added.

  * `rb::protect()`: Added.

  * `rb::cast<std::string>(rb::Object)`: Added.

  * `rb::cast<rb::Object(std::string)`: Added.

  * Added support for auto needless C++ compiler warning flags remove.

## 0.0.3 - 2018-02-18

### Improvements

  * Added macOS support.
    [GitHub#2][Reported by Paul Suk]

### Thanks

  * Paul Suk

## 0.0.2 - 2018-02-16

### Improvements

  * `rb::Object::ivar_set()`: Added.

  * `rb::Object::ivar_get()`: Added.

### Fixes

  * Added missing files to gem.

## 0.0.1 - 2018-02-16

The first release!!!
