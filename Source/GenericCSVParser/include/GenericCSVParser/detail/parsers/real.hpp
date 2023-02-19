#pragma once

#include <boost/spirit/home/x3/numeric/real.hpp>
#include <boost/hana/string.hpp>

namespace GenericCSVParser::CSV {
    struct real {
        static constexpr auto name_ = boost::hana::string_c<'r', 'e', 'a', 'l',
                ' ', 'n', 'u', 'm', 'b', 'e', 'r'>;
    };

    template<char Separator>
    static inline auto as_parser(real)
    {
        namespace x3 = boost::spirit::x3;
        return x3::rule<struct real_, double>{real::name_.c_str()} =
                       x3::double_;
    }
}
