#pragma once

#include <boost/hana/string.hpp>
#include <boost/spirit/home/x3/string/symbols.hpp>

namespace GenericCSVParser::CSV {
    struct boolean {
        static constexpr auto name_ =
                boost::hana::string_c<'b', 'o', 'o', 'l', 'e', 'a', 'n'>;
    };

    namespace detail {
        struct booleans_ : boost::spirit::x3::symbols<bool> {
            booleans_()
            {
                // clang-format off
                add("true", true)("True", true)("TRUE", true)
                   ("T", true)("t", true)("1", true)
                   ("Yes", true)("yes", true)("Y", true)("y", true)
                   ("false", false)("False", false)("FALSE", false)
                   ("F", false)("f", false)("0", false)
                   ("No", false)("no", false)("N", false)("n", false)
                   ;
                // clang-format on
            }
        } booleans;
    }
    template<char Separator>
    static inline auto as_parser(boolean)
    {
        namespace x3 = boost::spirit::x3;
        return x3::rule<struct boolean_, bool>{boolean::name_.c_str()} =
                       detail::booleans;
    }
}
