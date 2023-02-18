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
        std::cout << e.a_ << " " << e.b_ << " " << e.c_ << " " << e.d_.year()
                  << '-' << e.d_.month() << '-' << e.d_.day() << '\n';
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
