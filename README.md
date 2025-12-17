# Modern C++ Lab: Error Handling

This repository implements a lightweight error-handling framework named `Flex`, based on `std::expected` and C++ `concept`.

## Development Environment

- Compiler: GCC 14
- Build System: CMake 4.1.2, Ninja 1.13.1

> `Ninja` is optional; you can also use `Makefile`.

## Build

```shell
# Create build directory
mkdir build
cd build

# Generate build scripts
cmake .. -G Ninja

# Build
ninja
```

## Testing

- `Flex` performance test: `./build/test/test_flex_perf`
