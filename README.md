# Xendit C/C++ SDK

The **unofficial** Xendit Go SDK provides a simple and convenient way to call
Xendit's REST API in applications written in C/C++.

- Package version:

# Getting Started

## Prerequisites

- [The GNU Compiler Collection](https://gcc.gnu.org)
- [CMake](https://cmake.org)

## Dependencies

- [libcurl](https://curl.se/libcurl)
- [json-c](https://json-c.github.io/json-c/)

## Compiling and Installation

For an out-of-source build, create a build directory and change to it:

```bash
mkdir build && cd build
```

Run CMake to configure the build system and compile:

```bash
cmake .. && make
```

Or to build it with debugging symbols and disabled compiler optimizations, run:

```bash
cmake -DCMAKE_BUILD_TYPE=Debug .. && make
```

To install it on the system, run:

```bash
make install
```

## Testing

### Prerequisites

- [CTest](https://cmake.org/cmake/help/latest/module/CTest.html) - Part of
  CMake package
- [Valgrind](https://valgrind.org/) - Optional, for leak detection.

The testing is integrated with the CTest testing suite. Additionally, memory
leak checking is integrated with Valgrind using the following flags:
`--leak-check=full --error-exitcode=1`, ensuring that any leaks will be
detected by CTest through the exit code.

[Read more about testing with CTest](https://cmake.org/cmake/help/latest/module/CTest.html)

## Authorization

The SDK needs to be instantiated using your secret API key obtained from the
[Xendit Dashboard](https://dashboard.xendit.co/settings/developers#api-keys).
You can sign up for a free Dashboard account
[here](https://dashboard.xendit.co/register).

```c
#include <xendit/xendit.h>

xnd_client_t *client = xnd_client_new("XENDIT_API_KEY");
```

```c++
#include <xendit/xendit.hpp>

xendit::client client { "XENDIT_API_KEY" };
```

## Compiling Your Program with Xendit C/C++ SDK

[TODO]

# Documentation

All URIs are relative to [https://api.xendit.co](https://api.xendit.co). For
more information about our API, please refer to
[https://developers.xendit.co/](https://developers.xendit.co).

Further Reading

- [Xendit Docs](https://docs.xendit.co)
- [Xendit API Reference](https://developers.xendit.co)
