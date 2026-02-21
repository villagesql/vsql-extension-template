# AGENTS.md

This file provides guidance to AI coding assistants when working with code in this repository.

**Note**: Also check `AGENTS.local.md` for additional local development instructions when present.

## Project Overview

This is a template project for creating VillageSQL extensions. It provides the minimum required files and structure to build, package, and install custom extensions for VillageSQL (a MySQL-compatible database). The template includes a simple "Hello, World!" function as an example.

## Build System

- **Build**: `cmake . && make` (or `mkdir build && cd build && cmake .. && make`)
- **Create VEB package**: Automatically created during `make`
- **Install extension**: `make install` (if VillageSQL_VEB_INSTALL_DIR is defined)

The build process:
1. Uses CMake to find VillageSQL via `find_package(VillageSQL REQUIRED)`
2. Compiles C++ source files into shared library `hello.so`
3. Packages library with `manifest.json` into `vsql_extension_template.veb` archive using `VEF_CREATE_VEB()` macro
4. VEB can be installed and loaded using `INSTALL EXTENSION` command

Set `VillageSQL_BUILD_DIR` to point to your VillageSQL build directory.

## Architecture

**Core Components:**
- `src/hello.cc` - VEF function implementation for the hello_world function
- `manifest.json` - Extension metadata (name, version, description, author, license)
- `CMakeLists.txt` - CMake build configuration
- `cmake/FindVillageSQL.cmake` - CMake module for finding VillageSQL
- `test/t/` - Test files directory (`.test` files using MTR framework)
- `test/r/` - Expected test results directory (`.result` files)

**Available Functions:**
- `hello_world()` - Returns the string "Hello, World!"

**Dependencies:**
- Requires VillageSQL (Extension Framework headers)
- Uses VillageSQL Extension Framework (VEF) API
- C++ compiler with C++17 support

**Code Organization:**
- File naming: lowercase with underscores (e.g., `hello.cc`)
- Function naming: lowercase with underscores (e.g., `hello_world`)
- Extension naming: lowercase with underscores (e.g., `vsql_extension_template`)
- Variable naming: lowercase with underscores (e.g., `result`)

## VillageSQL Extension Framework (VEF) API Pattern

VEF provides a modern C++ API for creating extensions. Functions are registered using the `VEF_GENERATE_ENTRY_POINTS()` macro with a fluent builder interface.

### Basic Function Implementation

Each function is implemented as a single function that takes context and result parameters:

```cpp
#include <villagesql/extension.h>

using namespace villagesql::extension_builder;
using namespace villagesql::func_builder;

// Function implementation
void my_function_impl(vef_context_t* ctx, vef_vdf_result_t* result) {
    // Implement function logic
    // Set result->type (IS_VALUE, IS_NULL, IS_ERROR)
    // For strings: copy to result->str_buf, set result->actual_len
    // For integers: set result->int_value
    // For binary: copy to result->bin_buf, set result->actual_len
}
```

### Function with Arguments

For functions with arguments, add `vef_invalue_t*` parameters:

```cpp
void my_function_impl(vef_context_t* ctx,
                      vef_invalue_t* arg1,
                      vef_invalue_t* arg2,
                      vef_vdf_result_t* result) {
    // Check for NULL: arg1->is_null
    // Access string: arg1->str_value, arg1->str_len
    // Access integer: arg1->int_value
    // Access binary: arg1->bin_value, arg1->bin_len
}
```

### Extension Registration

Use the `VEF_GENERATE_ENTRY_POINTS()` macro to register the extension and its functions:

```cpp
VEF_GENERATE_ENTRY_POINTS(
  make_extension("extension_name", "version")
    .func(make_func<&my_function_impl>("my_function")
      .returns(STRING)  // or INT, REAL, UUID, etc.
      .param(STRING)    // Add .param() for each argument
      .buffer_size(100) // For STRING return type
      .build())
)
```

### Result Types

- `IS_VALUE` - Function returned a value
- `IS_NULL` - Function returned NULL
- `IS_ERROR` - Function encountered an error (set `result->error_msg`)

## Testing

The extension includes test files using the MySQL Test Runner (MTR) framework:
- **Test Location**:
  - `test/t/` directory contains `.test` files with SQL test commands
  - `test/r/` directory contains `.result` files with expected output
- **Run Tests**:
  ```bash
  cd <BUILD_DIR>/mysql-test
  perl mysql-test-run.pl --suite=<path-to-vsql-extension-template>/test
  ```
  Where `<BUILD_DIR>` is your VillageSQL/MySQL build directory
- **Create/Update Results**: Use `--record` flag to generate or update expected `.result` files:
  ```bash
  perl mysql-test-run.pl --suite=<path-to-test-dir> --record
  ```
- Tests should validate function output and behavior
- Each test should install the extension, run tests, and clean up (drop functions, uninstall extension)

## Extension Installation

After building the VEB file, load the extension in VillageSQL:

```sql
INSTALL EXTENSION 'vsql_extension_template';
```

Then test the functions:
```sql
SELECT hello_world();
```

Note: Extension names use underscores, not hyphens (e.g., `vsql_extension_template`, not `vsql-extension-template`).

## Customizing the Template

To create your own extension:

1. **Update `manifest.json`**:
   - Change `name` (use underscores, e.g., `my_extension_name`)
   - Update `description`, `author`, and `version`

2. **Update `CMakeLists.txt`**:
   - Change `EXTENSION_NAME` variable (use underscores)
   - Update library name in `add_library()` if desired
   - Add additional source files to `add_library()` if needed
   - Add dependencies (e.g., `find_package(OpenSSL)`, `target_link_libraries()`)

3. **Implement your functions**:
   - Modify `src/hello.cc` or create new `.cc` files
   - Follow the VEF API pattern (single implementation function)
   - Use `VEF_GENERATE_ENTRY_POINTS()` to register functions
   - Add copyright header to all new source files

4. **Register functions in code**:
   - Functions are registered using the fluent builder API in `VEF_GENERATE_ENTRY_POINTS()`
   - Specify return type with `.returns(STRING|INT|REAL|...)`
   - Add parameters with `.param(type)`
   - Set buffer size for STRING returns with `.buffer_size(N)`
   - No separate `install.sql` file is needed

5. **Create tests**:
   - Add `.test` files in `test/t/` directory
   - Generate expected results in `test/r/` using `--record` flag
   - Each test should install extension, test functions, and clean up
   - Verify function behavior with various inputs

## Licensing and Copyright

All source code files (`.cc`, `.h`, `.cpp`, `.hpp`) and CMake files (`CMakeLists.txt`) must include the following copyright header at the top of the file:

```
/* Copyright (c) 2025 VillageSQL Contributors
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <https://www.gnu.org/licenses/>.
 */
```

When creating new source files, always include this copyright block before any code or includes.

## Common Tasks for AI Agents

When asked to add functionality to this template:

1. **Adding a new function**:
   - Create implementation function with signature: `void func_impl(vef_context_t* ctx, [args...], vef_vdf_result_t* result)`
   - Add function to `VEF_GENERATE_ENTRY_POINTS()` block using `.func(make_func<&func_impl>("name")...)`
   - Specify return type, parameters, and buffer size
   - Add to CMakeLists.txt if creating new source file
   - Create tests

2. **Modifying build**:
   - Edit CMakeLists.txt, ensure proper library linking
   - Use `target_link_libraries()` for dependencies

3. **Adding dependencies**:
   - Update CMakeLists.txt with `find_package()` or `target_link_libraries()`
   - Example: OpenSSL requires `find_package(OpenSSL REQUIRED)` and `target_link_libraries(hello PRIVATE ${OPENSSL_LIBRARIES})`

4. **Testing**:
   - Create or update `.test` files in `test/t/` directory
   - Generate expected results in `test/r/` using `--record`
   - Use extension name with underscores in `INSTALL EXTENSION` commands

5. **Documentation**:
   - Update README.md to reflect new functionality

**Important Conventions:**
- Always use underscores in extension names (not hyphens)
- Always include proper copyright headers in source files
- Use C++17 standard
- Functions are registered in code using VEF API (no install.sql needed)
- Result types: IS_VALUE, IS_NULL, IS_ERROR
