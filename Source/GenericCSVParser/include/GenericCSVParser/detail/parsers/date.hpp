#pragma once

#include <boost/spirit/home/x3/numeric/uint.hpp>
#include <boost/fusion/sequence/intrinsic/at.hpp>
#include <boost/date_time/gregorian/greg_date.hpp>
#include <boost/hana/string.hpp>

namespace GenericCSVParser::CSV {
    struct date {
        static constexpr auto name_ = boost::hana::string_c<'d', 'a', 't', 'e'>;
    };

    template<char Separator>
    static inline auto as_parser(date)
    {
        namespace x3 = boost::spirit::x3;
        static const x3::uint_parser<int, 10, 2, 2> uint2_;
        static const x3::uint_parser<int, 10, 4, 4> uint4_;

        static const auto to_date = [](auto& ctx) {
            using boost::fusion::at_c;
            auto& t = _attr(ctx);
            _val(ctx) =
                    boost::gregorian::date(at_c<0>(t), at_c<1>(t), at_c<2>(t));
        };
        return x3::rule<struct date_, boost::gregorian::date>{
                       date::name_.c_str()} =
                       (uint4_ >> '-' >> uint2_ >> '-' >> uint2_)[to_date];
    }
}
