#pragma once
#include <type_traits>

namespace GenericCSVParser::CSV {
    template<char Separator, typename T>
    using RuleParser =
            std::remove_cv_t<decltype(as_parser<Separator>(std::declval<T>()))>;

    template<char Separator, typename T>
    using RuleAttribute = typename RuleParser<Separator, T>::attribute_type;
}
