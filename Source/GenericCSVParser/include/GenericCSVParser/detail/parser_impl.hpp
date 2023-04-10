#pragma once

#include <boost/spirit/home/x3/core/parse.hpp>
#include <boost/spirit/home/x3/char/char_class.hpp>
#include <boost/spirit/home/x3/directive/with.hpp>
#include "generic_csv_parser.hpp"
#include <iosfwd>

namespace GenericCSVParser::detail {
    template<typename ParserConfig, typename Iterator>
    void parse(ParserConfig parserConfig, Iterator begin, Iterator end,
            std::ostream& errOut, typename ParserConfig::ReturnType& input)
    {
        using namespace CSV;

        using error_handler_type = x3::error_handler<Iterator>;
        // Our error handler
        error_handler_type error_handler(begin, end, errOut);
        // Our parser
        const auto parser = parserConfig.Parser();

        auto const p =
                // we pass our error handler to the parser, so we can access
                // it later in our on_error and on_success handlers
                x3::with<x3::error_handler_tag>(
                        std::ref(error_handler))[parser];
        if (x3::parse(begin, end, p, input))
        {
            if (begin != end)
                throw std::runtime_error("Remaining unparsed");
        }
        else
            throw std::runtime_error("Parse failed");
    }
    template<typename ParserConfig, typename Iterator>
    auto parse(ParserConfig parserConfig, Iterator begin, Iterator end,
            std::ostream& errOut) -> typename ParserConfig::ReturnType
    {
        typename ParserConfig::ReturnType input;
        parse(parserConfig, begin, end, errOut, input);
        return input;
    }
}
