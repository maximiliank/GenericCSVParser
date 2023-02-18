#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>
#include <string>
#include <optional>
#include "GenericCSVParser/parser.h"
#include "GenericCSVParser/parser_configuration.h"
#include "GenericCSVParser/detail/parsers/text.h"
#include "GenericCSVParser/detail/parsers/real.hpp"

#include "GenericCSVParser/detail/parsers/skip.h"
#include <sstream>

struct TestInputSkip {
    std::string a_;
    double c_;

    bool operator==(const TestInputSkip&) const = default;
};

#include <boost/fusion/include/define_struct.hpp>
// clang-format off
BOOST_FUSION_ADAPT_STRUCT(TestInputSkip,
                          (auto, a_)(auto, c_)
)
// clang-format on
namespace {
    class TestInputSkipParser : public ::testing::Test {

    protected:
        template<char Separator>
        static constexpr auto getConfig()
        {
            using namespace GenericCSVParser::CSV;

            return createParserConfiguration<Separator, TestInputSkip>(
                    Column<text>("Field1"), Column<skip>("Field2"),
                    Column<real>("Field3"), Column<skip>("Field4"));
        }

        template<char Separator>
        auto test(const std::string& input, std::ostream& errOut)
        {
            constexpr auto parserConfig =
                    TestInputSkipParser::getConfig<Separator>();
            return GenericCSVParser::CSVReader::parse(
                    parserConfig, input, errOut);
        }
    };

    TEST_F(TestInputSkipParser, test01)
    {
        using ::testing::ContainerEq;

        std::stringstream ss;
        const std::string input = R"(Field1;Field2;Field3;Field4
;0.5;1.0;2020-12-31
example;1;1.0;2021-12-31
test;3.4;1.0;2022-12-31)";
        auto data = test<';'>(input, std::cout);
        std::vector<TestInputSkip> expected = {TestInputSkip{"", 1.},
                TestInputSkip{"example", 1.}, TestInputSkip{"test", 1.}};
        EXPECT_THAT(data, ContainerEq(expected));
    }
}
