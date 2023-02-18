set(TARGET_NAME GenericCSVParserLib)

file(GLOB_RECURSE headers include/GenericCSVParser/*.h include/GenericCSVParser/*.hpp)

add_library(${TARGET_NAME} INTERFACE ${sources} ${headers})
target_include_directories(${TARGET_NAME} SYSTEM INTERFACE include/)

# In case this library is included in another project and project_options is not defined and make sure C++20 is enabled
if (NOT TARGET project_options)
    add_library(project_options INTERFACE)
    target_compile_features(project_options INTERFACE cxx_std_20)
endif ()
target_link_libraries(${TARGET_NAME} INTERFACE Boost::system Boost::iostreams fmt::fmt project_options)
