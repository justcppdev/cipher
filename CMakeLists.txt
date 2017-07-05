cmake_minimum_required(VERSION 3.5.2)

include("gate/cmake/HunterGate.cmake")
HunterGate(
    URL "https://github.com/ruslo/hunter/archive/v0.18.52.tar.gz"
    SHA1 "b16c19e28aefbb83b02aa90bacf8350fc45ef7d0"
)

project(cryptor)
include("config.cmake")
include("hunter.cmake")
include(CTest)

set(HEADERS cipher.hpp password.hpp wipe.hpp)

string(REGEX REPLACE "([^;]+)" "${CMAKE_SOURCE_DIR}/sources/\\1"
    HEADERS
 "${HEADERS}")

include_directories("include")
add_subdirectory("sources")

if(BUILD_TESTING)
    include_directories("sources")
    add_subdirectory("tests")
endif()
