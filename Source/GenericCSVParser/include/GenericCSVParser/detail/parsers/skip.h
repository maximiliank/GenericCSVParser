#pragma once
#include <boost/spirit/home/x3/directive/omit.hpp>
#include <boost/spirit/home/x3/char/char_parser.hpp>
#include <boost/spirit/home/x3/char/negated_char_parser.hpp>
#include <boost/hana/string.hpp>

namespace GenericCSVParser::CSV {
    struct skip {
        static constexpr auto name_ = boost::hana::string_c<'s', 'k', 'i', 'p'>;
    };

    template<char Separator>
    static inline auto as_parser(skip)
    {
        using namespace boost::spirit::x3;
        return x3::rule<struct skip_>{skip::name_.c_str()} =
                       omit[*(char_ - Separator - '\n')];
    }
}
