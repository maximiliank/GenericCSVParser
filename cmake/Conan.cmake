macro(run_conan)
    list(APPEND CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})
    list(APPEND CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR})

    if (NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
        message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
        file(DOWNLOAD "https://github.com/conan-io/cmake-conan/raw/0.18.1/conan.cmake"
                "${CMAKE_BINARY_DIR}/conan.cmake"
                TLS_VERIFY ON)
    endif ()

    include(${CMAKE_BINARY_DIR}/conan.cmake)

    # For multi configuration generators, like VS and XCode
    if (NOT CMAKE_CONFIGURATION_TYPES)
        message(STATUS "Single configuration build!")
        set(LIST_OF_BUILD_TYPES ${CMAKE_BUILD_TYPE})
        set(CMAKE_FIND_PACKAGE_GENERATOR cmake_find_package)
    else ()
        message(STATUS "Multi-configuration build: '${CMAKE_CONFIGURATION_TYPES}'!")
        set(LIST_OF_BUILD_TYPES ${CMAKE_CONFIGURATION_TYPES})
        set(CMAKE_FIND_PACKAGE_GENERATOR cmake_find_package_multi)
    endif ()

    conan_cmake_configure(
            REQUIRES
            ${CONAN_EXTRA_REQUIRES}
            fmt/9.1.0
            boost/1.80.0
            OPTIONS
            ${CONAN_EXTRA_OPTIONS}
            GENERATORS ${CMAKE_FIND_PACKAGE_GENERATOR})

    foreach (TYPE ${LIST_OF_BUILD_TYPES})
        message(STATUS "Running Conan for build type '${TYPE}'")
        conan_cmake_autodetect(settings BUILD_TYPE ${TYPE})
        conan_cmake_install(PATH_OR_REFERENCE .
                BUILD ${CONAN_BUILD_OPTION}
                REMOTE conancenter
                SETTINGS ${settings})
    endforeach ()
endmacro()
