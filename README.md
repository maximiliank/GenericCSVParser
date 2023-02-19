Generic CSV Parser
==================

This is a library that should make it easier to parse csv files into a vector of structs using `boost::spirit::x3`. The
key idea is taken from the following post https://stackoverflow.com/a/50824603.

You can specify header column names and parser types in a configuration and then parse in to `boos::fusion` adapted
struct, e.g.

```c++
# examples/csvparser.cpp
```

Usage via FetchContent
----------------------

It is also possible to include the required subdirectories using `FetchContent_Declare`. This ensures that the same
compiler flags are used.
You should only include the sub-folder containing the source of `GenericCSVParser`.
You can create a file `cmake/GenericCSVParser.cmake` with the following content:

```cmake
include(FetchContent)
FetchContent_Declare(
        generic_csv_parser
        GIT_REPOSITORY https://github.com/maximiliank/GenericCSVParser.git
        GIT_TAG origin/main
        UPDATE_DISCONNECTED ON
)

FetchContent_GetProperties(generic_csv_parser)
if (NOT generic_csv_parser_POPULATED)
    FetchContent_Populate(generic_csv_parser)
endif ()
```

and then include it in your `CMakeLists.txt`

```cmake
include(cmake/GenericCSVParser.cmake)
add_subdirectory(${generic_csv_parser_SOURCE_DIR}/Source/GenericCSVParser ${generic_csv_parser_BINARY_DIR}_GenericCSVParser)
```
