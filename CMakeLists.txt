cmake_minimum_required(VERSION 3.16)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED YES)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED YES)

project(patterns
    VERSION     1.0
    DESCRIPTION "patterns, containers, algorithms and tests"
    LANGUAGES   CXX
)

# Use the timestamp of the extracted files, after extraction,
# as the source dependency up-to-date-ness.
cmake_policy(SET CMP0135 NEW)

# Download googletest:
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)

# Build googletest libraries:
FetchContent_MakeAvailable(googletest)

# "Debug" -> "DEBUG", "Release" -> "RELEASE", ...
string(TOUPPER ${CMAKE_BUILD_TYPE} BUILD_TYPE_UPPER)

# This really should be done within a toolchain file. But for now...
set(CMAKE_CXX_FLAGS_${BUILD_TYPE_UPPER}
    ${CMAKE_CXX_FLAGS_${BUILD_TYPE_UPPER}}
    -Wall
    -Wpedantic
    -Wextra
    -Wformat=2
    -Wswitch-default
    -Wpointer-arith
    -Wstrict-overflow=5
    -Winline
    -Wundef
    -Wcast-qual
    -Wshadow
    -Wunreachable-code
    -Wredundant-decls
    -fstrict-aliasing
    -fstrict-overflow
    -fstack-protector-all
    -Wmissing-field-initializers
    -Wno-unknown-pragmas
    -Wno-enum-compare
    -Werror
)

# CMake does not properly convert semicolon separated strings into a whitespace
# separated argument list when used by compiler and linker options.
macro(semicolons_to_spaces string_var)
    string(REPLACE ";" " " ${string_var} "${${string_var}}")
endmacro()

semicolons_to_spaces(CMAKE_CXX_FLAGS_${BUILD_TYPE_UPPER})

if(NOT DEFINED CMAKE_BUILD_TYPE OR "${CMAKE_BUILD_TYPE}" STREQUAL "")
    set(CMAKE_BUILD_TYPE "Debug")
    message(STATUS "Setting to default CMAKE_BUILD_TYPE : ${CMAKE_BUILD_TYPE}")
endif()

include_directories(
    ${CMAKE_SOURCE_DIR}
)

enable_testing()

# Bug in CMake fetched content?
# Explicitly add header file directory inclusion for googlemock/include.
# Even though googletest/include does get found, add it also for symmetry.
FetchContent_GetProperties(googletest SOURCE_DIR GOOGLE_TEST_SOURCE_DIR)

function(add_test_executable test_name)

    add_executable(${test_name} ${ARGV})

    target_include_directories(
        ${test_name}
        AFTER
        PRIVATE
        ${GOOGLE_TEST_SOURCE_DIR}/googletest/include
        ${GOOGLE_TEST_SOURCE_DIR}/googlemock/include
    )

    target_link_libraries(
        ${test_name}
        GTest::gtest_main
        GTest::gmock_main
    )

endfunction()

# add_test_executable(
#     test_linked_list
#     test_linked_list.cc
# )

# add_test_executable(
#     test_binary_tree
#     binary_tree/test_binary_tree.cc
# )

add_executable(test_binary_tree binary_tree/test_binary_tree.cc)

target_include_directories(
    test_binary_tree
    AFTER
    PRIVATE
    ${GOOGLE_TEST_SOURCE_DIR}/googletest/include
    ${GOOGLE_TEST_SOURCE_DIR}/googlemock/include
)

target_link_libraries(
    test_binary_tree
    GTest::gtest_main
    GTest::gmock_main
)

include(GoogleTest)
gtest_discover_tests(test_binary_tree)


message(STATUS "CMAKE_SYSTEM_NAME             : ${CMAKE_SYSTEM_NAME}")
message(STATUS "TOOLCHAIN                     : ${CMAKE_TOOLCHAIN_FILE}")
message(STATUS "CMAKE_BUILD_TYPE              : ${CMAKE_BUILD_TYPE}")
message(DEBUG  "RUNTIME_OUTPUT_DIRECTORY      : ${RUNTIME_OUTPUT_DIRECTORY}")
message(DEBUG  "CMAKE_CONFIGURATION_TYPES     : ${CMAKE_CONFIGURATION_TYPES}")
message(DEBUG  "C_FLAGS                       : ${CMAKE_C_FLAGS_${BUILD_TYPE_UPPER}}")
message(DEBUG  "CXX_FLAGS                     : ${CMAKE_CXX_FLAGS_${BUILD_TYPE_UPPER}}")
message(DEBUG  "CMAKE_OBJCOPY                 : ${CMAKE_OBJCOPY}")
message(DEBUG  "CMAKE_OBJDUMP                 : ${CMAKE_OBJDUMP}")
message(DEBUG  "CMAKE_NM                      : ${CMAKE_NM}")
message(DEBUG  "CMAKE_SIZE                    : ${CMAKE_SIZE}")
message(DEBUG  "GOOGLE_TEST_SOURCE_DIR        : ${GOOGLE_TEST_SOURCE_DIR}")

# add_subdirectory(binary_tree)
# add_subdirectory(linked_list)
# add_subdirectory(observer)
