#pragma once

#include <boost/spirit/home/x3/directive/lexeme.hpp>
#include <boost/hana/string.hpp>
#include "resolve_rule.hpp"

namespace GenericCSVParser::CSV {

    template<typename Parser>
    struct quoted {
        static constexpr auto name_ =
                boost::hana::string_c<'q', 'u', 'o', 't', 'e', 'd'> +
                Parser::name_;
    };

    namespace internal {
        template<typename>
        struct quoted_;
    }
    template<char Separator, typename Parser>
    static inline auto as_parser(quoted<Parser>)
    {
        using namespace boost::spirit::x3;
        return rule<internal::quoted_<Parser>,
                       RuleAttribute<Separator, Parser>>{
                       quoted<Parser>::name_.c_str()} =
                       lexeme['"' >> as_parser<Separator>(Parser{}) >> '"'];
    }
}
