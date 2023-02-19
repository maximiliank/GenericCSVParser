#pragma once

#include <boost/spirit/home/x3/core/parser.hpp>
#include <boost/spirit/home/x3/core/action.hpp>
#include <boost/spirit/home/x3/nonterminal/rule.hpp>
#include <boost/spirit/home/x3/directive/omit.hpp>
#include <boost/spirit/home/x3/directive/lexeme.hpp>
#include <boost/spirit/home/x3/directive/expect.hpp>
#include <boost/spirit/home/x3/directive/skip.hpp>
#include <boost/spirit/home/x3/numeric/uint.hpp>
#include <boost/spirit/home/x3/numeric/int.hpp>
#include <boost/spirit/home/x3/numeric/real.hpp>
#include <boost/spirit/home/x3/char/char_parser.hpp>
#include <boost/spirit/home/x3/char/char.hpp>
#include <boost/spirit/home/x3/operator/optional.hpp>
#include <boost/spirit/home/x3/operator/difference.hpp>
#include <boost/spirit/home/x3/operator/plus.hpp>
#include <boost/spirit/home/x3/operator/and_predicate.hpp>
#include <boost/spirit/home/x3/operator/alternative.hpp>
#include <boost/spirit/home/x3/operator/kleene.hpp>
#include <boost/spirit/home/x3/operator/list.hpp>
#include <boost/spirit/home/x3/operator/sequence.hpp>
#include <boost/spirit/home/x3/auxiliary/eoi.hpp>
#include <boost/spirit/home/x3/auxiliary/eol.hpp>
#include <boost/spirit/home/x3/auxiliary/eps.hpp>

#include <boost/spirit/home/x3/support/utility/error_reporting.hpp>
#include <boost/spirit/home/x3/support/utility/annotate_on_success.hpp>
#include <boost/spirit/home/x3/string/literal_string.hpp>

#include <boost/hana/string.hpp>

namespace GenericCSVParser::CSV {
    namespace x3 = boost::spirit::x3;
    struct error_handler {
        template<typename Iterator, typename Exception, typename Context>
        x3::error_handler_result on_error(Iterator& first, Iterator const& last,
                Exception const& x, Context const& context)
        {
            auto& error_handler = x3::get<x3::error_handler_tag>(context).get();
            std::string message = "Error! Expecting: " + x.which() + " here:";
            error_handler(x.where(), message);
            return x3::error_handler_result::fail;
        }
    };

    struct CSVLine_t;
    struct CSVBody_t;
    struct CSVFile_t;

    template<char Separator>
    static inline auto constexpr delim()
    {
        constexpr auto name = boost::hana::string_c<'d', 'e', 'l', 'i', 'm',
                'i', 't', 'e', 'r', ' ', '\'', Separator, '\''>;
        return x3::rule<struct Delim>{name.c_str()} = x3::char_(Separator);
    }

    const auto lineend = &(x3::eoi | x3::eol);
    template<char Separator, typename CSVLine, typename First, typename... Rest>
    static inline auto line_parser(First first, Rest... rest)
    {
        return x3::rule<CSVLine_t, CSVLine>{"line"} =
                       x3::expect[as_parser<Separator>(first)] >>
                       ((x3::expect[delim<Separator>()] >>
                                x3::expect[as_parser<Separator>(rest)]) >>
                               ...) >>
                       x3::expect[lineend];
    }

    template<char Separator, typename CSVFile, typename... Spec>
    static inline auto csv_parser(Spec... spec)
    {
        const auto line =
                line_parser<Separator, typename CSVFile::value_type>(spec...);
        return x3::rule<CSVBody_t, CSVFile>{"body"} =
                       ((line - x3::eoi) % x3::eol) >> -x3::eol >> x3::eoi;
    }

    template<char Separator, typename CSVFile, typename... Spec>
    static inline auto csv_parser_with_header(
            std::array<const char*, sizeof...(Spec)> header, Spec... spec)
    {
        auto headerParser = [](auto first, auto&&... rest) {
            return x3::rule<struct Header_t>{"header"} =
                           x3::expect[x3::as_parser(first)] >>
                           ((x3::expect[delim<Separator>()] >>
                                    x3::expect[x3::as_parser(rest)]) >>
                                   ...);
        };
        auto createHeaderParser = [](auto&& fn, const auto& u) {
            return std::apply([fn = std::forward<decltype(fn)>(fn)](
                                      auto... item) { return fn(item...); },
                    u);
        };
        const auto headerRule = createHeaderParser(headerParser, header);
        const auto bodyRule = csv_parser<Separator, CSVFile>(spec...);

        return x3::rule<CSVFile_t, CSVFile>{"file"} =
                       headerRule >> x3::expect[x3::eol] >> bodyRule;
    }

    using lineend_type = std::remove_cv_t<decltype(lineend)>;

    struct CSVLine_t /*: x3::annotate_on_success*/ {};
    struct CSVBody_t /*: x3::annotate_on_success*/ {};
    struct CSVFile_t : error_handler /*, x3::annotate_on_success*/ {};
}
namespace boost::spirit::x3 {
    template<>
    struct get_info<GenericCSVParser::CSV::lineend_type> {
        typedef std::string result_type;
        std::string operator()(GenericCSVParser::CSV::lineend_type const&) const
        {
            return "end of line or input";
        }
    };
}
