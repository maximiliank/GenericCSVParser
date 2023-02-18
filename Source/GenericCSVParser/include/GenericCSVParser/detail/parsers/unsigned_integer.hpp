#pragma once

#include <boost/spirit/home/x3/numeric/uint.hpp>
#include <boost/hana/string.hpp>

namespace GenericCSVParser::CSV {
    struct unsigned_integer {
        static constexpr auto name_ = boost::hana::string_c<'u', 'n', 's', 'i',
                'g', 'n', 'e', 'd', ' ', 'i', 'n', 't', 'e', 'g', 'e', 'r'>;
    };

    template<char Separator>
    static inline auto as_parser(unsigned_integer)
    {
        namespace x3 = boost::spirit::x3;
        return x3::uint_;
    }
}
namespace boost::spirit::x3 {
    template<>
    struct get_info<uint_type> {
        typedef std::string result_type;
        std::string operator()(uint_type const&) const
        {
            return GenericCSVParser::CSV::unsigned_integer::name_.c_str();
        }
    };
}
