#include "GenericCSVParser/parser.h"
#include "GenericCSVParser/parser_configuration.h"
#include "GenericCSVParser/detail/parsers/text.h"
#include "GenericCSVParser/detail/parsers/real.hpp"
#include "GenericCSVParser/detail/parsers/optional.hpp"
#include "GenericCSVParser/detail/parsers/quoted.hpp"
#include "GenericCSVParser/detail/parsers/date.hpp"
#include "GenericCSVParser/detail/parsers/boolean.hpp"
#include "GenericCSVParser/detail/parsers/integer.hpp"
#include "GenericCSVParser/detail/parsers/unsigned_integer.hpp"
#include <iostream>
#include <optional>

#include <boost/fusion/include/define_struct.hpp>

// clang-format off
BOOST_FUSION_DEFINE_STRUCT((Parser)(Examples), TestInput,
		(std::string, a_)
		(double, b_)
        (bool, c_)
        (int, d_)
        (unsigned int, e_)
        (boost::gregorian::date, f_)
        (std::optional<bool>, g_)
        (std::string, h_)
        (double, i_))
// clang-format on

template<typename T>
std::ostream& operator<<(std::ostream& os, const std::optional<T>& opt)
{
    if (opt.has_value())
        os << opt.value();
    else
        os << "-NA-";
    return os;
}

template<typename T, typename Allocator>
void print(const std::vector<T, Allocator>& vec)
{
    for (const auto& e : vec)
    {
        std::cout << e.a_ << ' ' << e.b_ << ' ' << std::boolalpha << e.c_ << ' '
                  << e.d_ << ' ' << e.e_ << ' ' << e.f_.year() << '-'
                  << e.f_.month() << '-' << e.f_.day() << ' ' << e.g_ << ' '
                  << e.h_ << ' ' << e.i_ << '\n';
    }
    std::cout << std::endl;
}
int main()
{
    try
    {
        using namespace GenericCSVParser::CSV;
        // clang-format off
        auto parserConfig = createParserConfiguration<',', ::Parser::Examples::TestInput>(
        	Column<text>("Field1"),
        	Column<real>("Field2"),
            Column<boolean>("Field3"),
            Column<integer>("Field4"),
            Column<unsigned_integer>("Field5"),
            Column<date>("Field6"),
            Column<optional<boolean>>("Field7"),
            Column<quotedtext>("Field8"),
            Column<quoted<real>>("Field9")
        );
        // clang-format on
        auto data = GenericCSVParser::CSVReader::parse(parserConfig,
                R"(Field1,Field2,Field3,Field4,Field5,Field6,Field7,Field8,Field9
,0.5,1,-1,2,2020-12-31,true,"Long quoted text with comma ,","3.55"
"second",1,0,2,2,2021-12-31,,",,","0.1"
"third",3.4,true,0,0,2022-12-31,No,"","1e2")",
                std::cerr);
        print(data);

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
