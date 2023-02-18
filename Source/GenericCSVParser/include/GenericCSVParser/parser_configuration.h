#pragma once

#include <type_traits>
#include <vector>
#include <array>
#include "GenericCSVParser/detail/generic_csv_parser.hpp"

namespace GenericCSVParser::CSV {

    template<char Separator, typename T, typename... Args>
    struct ParserConfiguration {
        using ReturnType = std::vector<T>;
        using HeaderArray = std::array<const char*, sizeof...(Args)>;
        static constexpr char sep = Separator;

        HeaderArray header_;

        constexpr explicit ParserConfiguration(HeaderArray header)
            : header_(header)
        {}

        auto Parser()
        {
            return csv_parser_with_header<Separator, ReturnType>(
                    header_, Args{}...);
        }
    };

    template<typename T>
    struct Column {
        using ColumnParser = T;
        const char* fieldName_;

        constexpr explicit Column(const char* fieldName) : fieldName_(fieldName)
        {}
    };

    template<typename T, typename _ = void>
    struct is_csv_column : std::false_type {};

    template<typename T>
    struct is_csv_column<Column<T>> : std::true_type {};

    template<typename T>
    inline constexpr bool is_csv_column_v = is_csv_column<T>::value;

    template<char Separator, typename T, typename... Args>
    constexpr auto createParserConfiguration(Args&&... args)
    {
        static_assert(
                []<typename... Ts>(std::type_identity<std::tuple<Ts...>>) {
                    return (is_csv_column_v<Ts> && ...);
                }(std::type_identity<std::tuple<Args...>>{}));
        return ParserConfiguration<Separator, T,
                typename Args::ColumnParser...>{{{args.fieldName_...}}};
    }
}
