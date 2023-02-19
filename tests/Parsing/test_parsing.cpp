#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>
#include <string>
#include <optional>
#include "GenericCSVParser/parser.h"
#include "GenericCSVParser/parser_configuration.h"
#include "GenericCSVParser/detail/parsers/text.h"
#include "GenericCSVParser/detail/parsers/real.hpp"
#include "GenericCSVParser/detail/parsers/optional.hpp"
#include "GenericCSVParser/detail/parsers/quoted.hpp"
#include "GenericCSVParser/detail/parsers/date.hpp"
#include "GenericCSVParser/detail/parsers/boolean.hpp"

#include <sstream>

struct TestInput {
    std::string a_;
    std::optional<double> b_;
    double c_;
    boost::gregorian::date d_;
    bool e_;

    bool operator==(const TestInput&) const = default;
};

#include <boost/fusion/include/define_struct.hpp>
// clang-format off
BOOST_FUSION_ADAPT_STRUCT(TestInput,
                          (auto, a_)(auto, b_)(auto, c_)(auto, d_)(auto, e_)
)
// clang-format on
namespace {
    class TestInputParser : public ::testing::Test {

    protected:
        template<char Separator>
        static constexpr auto getConfig()
        {
            using namespace GenericCSVParser::CSV;

            return createParserConfiguration<Separator, TestInput>(
                    Column<text>("Field1"), Column<optional<real>>("Field2"),
                    Column<quoted<real>>("Field3"), Column<date>("Field4"),
                    Column<boolean>("Field5"));
        }

        template<char Separator>
        auto test(const std::string& input, std::ostream& errOut)
        {
            constexpr auto parserConfig =
                    TestInputParser::getConfig<Separator>();
            return GenericCSVParser::CSVReader::parse(
                    parserConfig, input, errOut);
        }
        template<char Separator>
        void test_fail(const std::string& input, const std::string& message)
        {
            std::stringstream ss;
            ASSERT_THROW(test<Separator>(input, ss), std::runtime_error);
            EXPECT_EQ(ss.str(), message);
        }
    };

    TEST_F(TestInputParser, test01)
    {
        using ::testing::ContainerEq;

        std::stringstream ss;
        const std::string input = R"(Field1,Field2,Field3,Field4,Field5
,0.5,"1.0",2020-12-31,true
example,1,"1.0",2021-12-31,false
test,3.4,"1.0",2022-12-31,true)";
        auto data = test<','>(input, ss);
        std::vector<TestInput> expected = {
                TestInput{
                        "", .5, 1., boost::gregorian::date(2020, 12, 31), true},
                TestInput{"example", 1., 1.,
                        boost::gregorian::date(2021, 12, 31), false},
                TestInput{"test", 3.4, 1., boost::gregorian::date(2022, 12, 31),
                        true}};
        EXPECT_THAT(data, ContainerEq(expected));
        test_fail<';'>(input,
                "In line 1:\nError! Expecting: delimiter ';' "
                "here:\nField1,Field2,Field3,Field4,Field5\n______^_\n");
    }
}
