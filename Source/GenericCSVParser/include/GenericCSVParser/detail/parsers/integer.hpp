#pragma once

#include <boost/spirit/home/x3/numeric/int.hpp>
#include <boost/hana/string.hpp>

namespace GenericCSVParser::CSV {
    struct integer {
        static constexpr auto name_ =
                boost::hana::string_c<'i', 'n', 't', 'e', 'g', 'e', 'r'>;
    };

    template<char Separator>
    static inline auto as_parser(integer)
    {
        namespace x3 = boost::spirit::x3;
        return x3::int_;
    }
}
namespace boost::spirit::x3 {
    template<>
    struct get_info<int_type> {
        typedef std::string result_type;
        std::string operator()(int_type const&) const
        {
            return GenericCSVParser::CSV::integer::name_.c_str();
        }
    };
}
