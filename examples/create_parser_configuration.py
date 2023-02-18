from convert_csv_header import print_parser_definition, Column

print_parser_definition('test.csv', 'TestInput',
                        mapping={'Field1': Column('text', 'a_', 'std::string'),
                                 'Field2': Column('optional<real>', 'b_', 'std::optional<double>'),
                                 'Field3': Column('quoted<real>', 'c_', 'double'),
                                 'Field4': Column('date', 'd_', 'boost::gregorian::date')
                                 }, namespace="Parser::Examples")
