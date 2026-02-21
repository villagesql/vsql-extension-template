# VillageSQL Extension Template

A minimal template project for creating VillageSQL extensions. This template provides the essential structure and files needed to develop, build, and test custom VillageSQL extensions.

## What This Is

This template demonstrates how to create a VillageSQL extension by implementing a simple "Hello, World!" function. It includes all the minimum required files and follows the VillageSQL extension framework (VEF) structure.

## Project Structure

```
vsql_extension_template/
├── manifest.json           # Extension metadata (name, version, description, etc.)
├── CMakeLists.txt         # CMake build configuration
├── cmake/
│   └── FindVillageSQL.cmake  # CMake module for finding VillageSQL
├── src/
│   └── hello.cc           # C++ implementation using VEF API
└── test/
    ├── t/                 # Test files (.test)
    │   └── hello_basic.test
    └── r/                 # Expected results (.result)
        └── hello_basic.result
```

## Prerequisites

- VillageSQL build directory (with completed build)
- CMake 3.16 or higher
- C++ compiler with C++17 support
- OpenSSL development libraries

📚 **Full Documentation**: Visit [villagesql.com/docs](https://villagesql.com/docs) for comprehensive guides on building extensions, architecture details, and more.

## Building the Extension

1. Create a build directory and configure:

   **Linux:**
   ```bash
   mkdir build
   cd build
   cmake .. -DVillageSQL_BUILD_DIR=$HOME/build/villagesql
   ```

   **macOS:**
   ```bash
   mkdir build
   cd build
   cmake .. -DVillageSQL_BUILD_DIR=~/build/villagesql
   ```

   **Note**: `VillageSQL_BUILD_DIR` should point to your VillageSQL build directory. The VEB install directory is automatically set to `${VillageSQL_BUILD_DIR}/veb_output_directory`.

2. Build the extension:

   ```bash
   make -j $(($(getconf _NPROCESSORS_ONLN) - 2))
   ```

   This creates the `vsql_extension_template.veb` package in the build directory.

3. Install the VEB (optional):

```bash
make install
```

This copies the VEB to the directory specified by `VillageSQL_VEB_INSTALL_DIR`. If not using `make install`, you can manually copy the VEB file to your desired location.

## Using the Extension

After building the VEB file, load the extension in VillageSQL:

```sql
INSTALL EXTENSION vsql_extension_template;
```

Then test the function:

```sql
SELECT hello_world();
-- Returns: Hello, World!
```

Note: Extension names use underscores, not hyphens (e.g., `vsql_extension_template`).

## Testing

The extension includes test files using the MySQL Test Runner (MTR) framework.

### Running Tests

**Option 1 (Default): Using installed VEB**

This method assumes you have successfully run `make install` to install the VEB to your veb_dir.

**Linux:**
```bash
cd $HOME/build/villagesql/mysql-test
perl mysql-test-run.pl --suite=/path/to/vsql-extension-template/test

# Run with specific options
perl mysql-test-run.pl --suite=/path/to/vsql-extension-template/test --parallel=auto
```

**macOS:**
```bash
cd ~/build/villagesql/mysql-test
perl mysql-test-run.pl --suite=/path/to/vsql-extension-template/test

# Run with specific options
perl mysql-test-run.pl --suite=/path/to/vsql-extension-template/test --parallel=auto
```

**Option 2: Using a specific VEB file**

Use this to test a specific VEB build without installing it first:

**Linux:**
```bash
cd $HOME/build/villagesql/mysql-test
VSQL_EXTENSION_TEMPLATE_VEB=/path/to/build/vsql_extension_template.veb \
  perl mysql-test-run.pl --suite=/path/to/vsql-extension-template/test
```

**macOS:**
```bash
cd ~/build/villagesql/mysql-test
VSQL_EXTENSION_TEMPLATE_VEB=/path/to/build/vsql_extension_template.veb \
  perl mysql-test-run.pl --suite=/path/to/vsql-extension-template/test
```

### Creating/Updating Test Results

To create or update expected test results:

**Linux:**
```bash
cd $HOME/build/villagesql/mysql-test
perl mysql-test-run.pl --suite=/path/to/test --record
```

**macOS:**
```bash
cd ~/build/villagesql/mysql-test
perl mysql-test-run.pl --suite=/path/to/test --record
```

## Customizing This Template

To create your own extension:

1. **Update `manifest.json`**:
   - Change `name` to your extension name (use underscores, e.g., `my_extension_name`)
   - Update `description`, `author`, and other metadata

2. **Update `CMakeLists.txt`**:
   - Change `EXTENSION_NAME` to match your extension (use underscores)
   - Update the library name and source files in `add_library()`
   - Add dependencies if needed (e.g., `find_package()`, `target_link_libraries()`)

3. **Implement Your Functions**:
   - Modify `src/hello.cc` or create new source files
   - Use the VDF API signature: `void func_impl(vef_context_t* ctx, [args...], vef_vdf_result_t* result)`
   - Register functions using `VEF_GENERATE_ENTRY_POINTS()` macro
   - Include `<villagesql/extension.h>`

4. **Create Tests**:
   - Add `.test` files in the `test/t/` directory
   - Generate expected results with `--record` flag
   - Verify your functions work correctly

## Extension Development Tips

- **Extension Naming**: Always use underscores in extension names, not hyphens
- **Return Types**: Common types are `STRING`, `INT`, `REAL`, or custom types
- **Result Types**: Set `result->type` to `VEF_RESULT_VALUE`, `VEF_RESULT_NULL`, or `VEF_RESULT_ERROR`
- **String Results**: Copy to `result->str_buf` and set `result->actual_len`
- **Error Handling**: Set `result->type = VEF_RESULT_ERROR` and copy message to `result->error_msg`
- **NULL Handling**: Check `arg->is_null` for input arguments
- **Testing**: Always test with various inputs including edge cases and NULL values

## Example: Adding a New Function

1. Add implementation to `src/hello.cc`:

```cpp
// Function implementation
void greet_impl(vef_context_t* ctx,
                vef_invalue_t* name_arg,
                vef_vdf_result_t* result) {
    if (name_arg->is_null) {
        result->type = VEF_RESULT_NULL;
        return;
    }

    snprintf(result->str_buf, 256, "Hello, %.*s!",
             (int)name_arg->str_len, name_arg->str_value);
    result->type = VEF_RESULT_VALUE;
    result->actual_len = strlen(result->str_buf);
}
```

2. Register in `VEF_GENERATE_ENTRY_POINTS()`:

```cpp
VEF_GENERATE_ENTRY_POINTS(
  make_extension("vsql_extension_template", "1.0.0")
    .func(make_func<&hello_world_impl>("hello_world")
      .returns(STRING)
      .buffer_size(14)
      .build())
    .func(make_func<&greet_impl>("greet")
      .returns(STRING)
      .param(STRING)
      .buffer_size(256)
      .build())
)
```

3. Rebuild and test:

   ```bash
   cd build
   make -j $(($(getconf _NPROCESSORS_ONLN) - 2))
   make install  # If VillageSQL_VEB_INSTALL_DIR is configured
   ```

   Then in VillageSQL:

   ```sql
   INSTALL EXTENSION vsql_extension_template;

   -- Call without prefix
   SELECT greet('VillageSQL');

   -- Or with explicit namespace
   SELECT vsql_extension_template.greet('VillageSQL');
   ```

## Troubleshooting

### Build Failures

**VillageSQL SDK not found:**
```bash
# Make sure VillageSQL_BUILD_DIR points to your build directory
# Linux:
cmake .. -DVillageSQL_BUILD_DIR=$HOME/build/villagesql

# macOS:
cmake .. -DVillageSQL_BUILD_DIR=~/build/villagesql
```

**OpenSSL not found:**
```bash
# macOS with Homebrew
brew install openssl@3
cmake .. -DVillageSQL_BUILD_DIR=~/build/villagesql -DWITH_SSL=/opt/homebrew/opt/openssl@3

```sql
INSTALL EXTENSION vsql_extension_template;
SELECT greet('VillageSQL');
```

### Extension Loading Issues

**Extension not found after installation:**
- Verify the VEB file was copied to the correct directory
- Check that `INSTALL EXTENSION extension_name` uses the correct name (underscores, not hyphens)
- Restart the VillageSQL server if needed

**Function not found:**
- Ensure the extension is installed: `SELECT * FROM INFORMATION_SCHEMA.EXTENSIONS;`
- Try using explicit namespace: `extension_name.function_name()`

## Resources

- [VillageSQL Documentation](https://villagesql.com/docs)
- [VillageSQL Extension Framework (VEF) Guide](https://villagesql.com/docs)
- [CMake Documentation](https://cmake.org/documentation/)

## License

This template is released under the GPL-2.0 license. See the license header in source files for details.

## Contributing

When creating extensions based on this template, ensure your code follows the same license and includes appropriate copyright notices.
