cmake_minimum_required(VERSION 3.5.2)

add_executable(
    ${CMAKE_PROJECT_NAME}_tests
    main.cpp
    wipe_test.cpp
    helper.cpp
    helper.hpp
    ${HEADERS})

target_link_libraries(
    ${CMAKE_PROJECT_NAME}_tests
    ${CMAKE_PROJECT_NAME}_lib
    ${CATCH}
    ${BOOST}
    ${OPENSSL})

# add_test(NAME ${CMAKE_PROJECT_NAME}_unit_tests COMMAND
#     ${CMAKE_PROJECT_NAME}_tests ${CATCH_OPTIONS}) 
# 
# add_test(NAME ${CMAKE_PROJECT_NAME}_func_tests COMMAND tox) 

set(ENV{PYTHON_PATH} ./)
set(PYTEST_OPTIONS -v -s --color=yes)
add_test(NAME current_test COMMAND py.test ${PYTEST_OPTIONS}
    tests/cipher_test.py
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
