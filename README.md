Generic CSV Parser
==================

This is a library that should make it easier to parse csv files into a vector of structs using `boost::spirit::x3`. The
key idea is taken from the following post https://stackoverflow.com/a/50824603.

You can specify header column names and parser types in a configuration and then parse in to `boos::fusion` adapted
struct, e.g.

```c++
#include "GenericCSVParser/parser.h"
#include "GenericCSVParser/parser_configuration.h"
#include "GenericCSVParser/detail/parsers/text.h"
#include "GenericCSVParser/detail/parsers/real.hpp"
#include "GenericCSVParser/detail/parsers/optional.hpp"
#include "GenericCSVParser/detail/parsers/quoted.hpp"
#include "GenericCSVParser/detail/parsers/date.hpp"
#include <iostream>
#include <optional>

#include <boost/fusion/include/define_struct.hpp>

// clang-format off
BOOST_FUSION_DEFINE_STRUCT((Parser)(Examples), TestInput,
		(std::string, a_)
		(std::optional<double>, b_)
		(double, c_)
		(boost::gregorian::date, d_))
// clang-format on

int main()
{
    try
    {
        using namespace GenericCSVParser::CSV;
        // clang-format off
auto parserConfig = createParserConfiguration<',', ::Parser::Examples::TestInput>(
	Column<text>("Field1"),
	Column<optional<real>>("Field2"),
	Column<quoted<real>>("Field3"),
	Column<date>("Field4")
);
        // clang-format on
        auto data = GenericCSVParser::CSVReader::parse(parserConfig,
                R"(Field1,Field2,Field3,Field4
,0.5,"1.0",2020-12-31
"lala",1,"1.0",2021-12-31
"hmm",3.4,"1.0",2022-12-31)",
                std::cerr);
        // Do something with data

        data = GenericCSVParser::CSVReader::parseFromFile(
                parserConfig, "test.csv", std::cerr);
        print(data);
    }
    catch (const std::exception& ex)
    {
        std::cout << "Error: " << ex.what() << std::endl;
    }
    return 0;
}
```

Usage via FetchContent
----------------------

It is also possible to include the required subdirectories using `FetchContent_Declare`. This ensures that the same
compiler flags are used.
You should only include the subfolder containing the source of `GenericCSVParser`.
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
add_subdirectory(${generic_csv_parser_SOURCE_DIR}/Source/GenericCSVParser ${generic_parser_BINARY_DIR}_GenericCSVParser)
```
