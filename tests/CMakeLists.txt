set(TARGET_NAME UnitTests)

file(GLOB_RECURSE sources *.cpp *.h *.hpp)

add_executable(${TARGET_NAME} ${sources})
target_link_libraries(${TARGET_NAME} PRIVATE project_options project_warnings
        GenericCSVParserLib GTest::GTest Boost::headers)
# add Source directory to enable testing of internal implementations
target_include_directories(${TARGET_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/Source)

add_test(NAME unit COMMAND ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}/${TARGET_NAME})
