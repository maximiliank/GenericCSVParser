from typing import Dict
import csv
from collections import namedtuple

Column = namedtuple('Column', ['parser', 'cppname', 'cpptype'])


def print_parser_definition(file_path: str, struct_name: str, mapping: Dict[str, Column],
                            namespace: str = ''):
    with open(file_path, 'r') as csvfile:
        csv_headings = csvfile.readline().strip('\n')
        delimiter = str(csv.Sniffer().sniff(csv_headings).delimiter)

    struct_members = [f"({item.cpptype}, {item.cppname})" for item in mapping.values()]
    struct_members_str = "\n\t\t".join(struct_members)
    namespace_def = namespace if namespace == "" else f'({")(".join(namespace.split("::"))})'
    struct_def = f"#include <boost/fusion/include/define_struct.hpp>\n\n" \
                 f"// clang-format off\n" \
                 f"BOOST_FUSION_DEFINE_STRUCT({namespace_def}, {struct_name},\n\t\t{struct_members_str})\n" \
                 f"// clang-format on\n"

    header = csv_headings.split(delimiter)
    col_def = 'Column<{}>("{}")'
    struct_fqn = struct_name if namespace == "" else f"::{namespace}::{struct_name}"
    columns = [col_def.format(mapping.get(field, ['skip'])[0], field) for field in header]
    config = f"auto parserConfig = createParserConfiguration<'{delimiter}', {struct_fqn}>(\n\t" + ",\n\t".join(
        columns) + "\n);"

    print(struct_def)
    print(f"using namespace GenericCSVParser::CSV;\n// clang-format off\n{config}\n// clang-format on\n")
