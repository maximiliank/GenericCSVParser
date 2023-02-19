#pragma once

#include <string>
#include <boost/spirit/home/x3/nonterminal/rule.hpp>
#include <boost/spirit/home/x3/directive/lexeme.hpp>
#include <boost/spirit/home/x3/char/char_parser.hpp>
#include <boost/spirit/home/x3/char/negated_char_parser.hpp>
#include <boost/hana/string.hpp>

namespace GenericCSVParser::CSV {

    struct text {
        static constexpr auto name_ = boost::hana::string_c<'u', 'n', 'q', 'u',
                'o', 't', 'e', 'd', ' ', 't', 'e', 'x', 't'>;
    };

    struct quotedtext {
        static constexpr auto name_ = boost::hana::string_c<'q', 'u', 'o', 't',
                'e', 'd', ' ', 't', 'e', 'x', 't'>;
    };

    template<char Separator>
    static inline auto as_parser(text)
    {
        return boost::spirit::x3::rule<struct unquoted_, std::string>{
                       text::name_.c_str()} =
                       *(boost::spirit::x3::char_ - Separator - '\n');
    }

    template<char Separator>
    static inline auto as_parser(quotedtext)
    {
        return boost::spirit::x3::rule<struct quoted_, std::string>{
                       quotedtext::name_.c_str()} = boost::spirit::x3::lexeme
                       ['"' >> *(boost::spirit::x3::char_ - '"') >> '"'];
    }
}
