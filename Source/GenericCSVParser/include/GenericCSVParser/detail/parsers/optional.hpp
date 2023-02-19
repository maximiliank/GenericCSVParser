#pragma once

#include <boost/spirit/home/x3/operator/optional.hpp>
#include <boost/hana/string.hpp>
#include <optional>
#include "resolve_rule.hpp"

namespace GenericCSVParser::CSV {

    template<typename Parser>
    struct optional {
        static constexpr auto name_ =
                boost::hana::string_c<'o', 'p', 't', 'i', 'o', 'n', 'a', 'l'> +
                Parser::name_;
    };

    namespace internal {
        template<typename>
        struct optional_;
    }

    template<char Separator, typename Parser>
    static inline auto as_parser(optional<Parser>)
    {
        using namespace boost::spirit::x3;
        return rule<internal::optional_<Parser>,
                       std::optional<RuleAttribute<Separator, Parser>>>{
                       optional<Parser>::name_.c_str()} =
                       -as_parser<Separator>(Parser{});
    }

}
