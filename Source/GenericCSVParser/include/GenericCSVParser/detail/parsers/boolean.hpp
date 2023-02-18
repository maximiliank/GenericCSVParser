#pragma once

#include <boost/spirit/home/x3/numeric/bool.hpp>
#include <boost/hana/string.hpp>

namespace GenericCSVParser::CSV {
    struct boolean {
        static constexpr auto name_ =
                boost::hana::string_c<'b', 'o', 'o', 'l', 'e', 'a', 'n'>;
    };

    template<char Separator>
    static inline auto as_parser(boolean)
    {
        namespace x3 = boost::spirit::x3;
        return x3::bool_;
    }
}
namespace boost::spirit::x3 {
    template<>
    struct get_info<bool_type> {
        typedef std::string result_type;
        std::string operator()(bool_type const&) const
        {
            return GenericCSVParser::CSV::boolean::name_.c_str();
        }
    };
}
