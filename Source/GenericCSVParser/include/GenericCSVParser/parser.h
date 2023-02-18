#pragma once

#include <string>
#include <string_view>
#include <boost/iostreams/device/mapped_file.hpp>
#include <boost/spirit/include/support_istream_iterator.hpp>
#include <fstream>
#include <iosfwd>
#include <fmt/format.h>
#include "detail/parser_impl.hpp"

namespace GenericCSVParser {

    class CSVReader {
    public:
        template<typename ParserConfig>
        static auto parse(ParserConfig parserConfig, std::string_view str,
                std::ostream& errOut)
        {
            return parse_impl(parserConfig, str, errOut);
        }

        template<typename ParserConfig>
        static auto parseFromMemoryMappedFile(ParserConfig parserConfig,
                const std::string& filename, std::ostream& errOut)
        {
            boost::iostreams::mapped_file_source csv(filename);
            return parse_impl(parserConfig, csv, errOut);
        }
        template<typename ParserConfig>
        static auto parseFromFile(ParserConfig parserConfig,
                const std::string& filename, std::ostream& errOut)
        {
            std::ifstream in(filename);
            if (!in.is_open() || !in.good())
                throw std::runtime_error(
                        fmt::format("Cannot open file {}", filename));
            return parse(parserConfig, in, errOut);
        }

        template<typename ParserConfig>
        static auto parse(ParserConfig parserConfig, std::istream& stream,
                std::ostream& errOut)
        {
            stream.unsetf(std::ios::skipws);

            boost::spirit::istream_iterator begin(stream);
            boost::spirit::istream_iterator end;

            return detail::parse(parserConfig, begin, end, errOut);
        }

    private:
        template<typename ParserConfig, typename Range>
        static auto parse_impl(
                ParserConfig parserConfig, Range&& range, std::ostream& errOut)
        {
            return detail::parse(
                    parserConfig, range.begin(), range.end(), errOut);
        }
    };
}
