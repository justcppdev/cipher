cmake_minimum_required(VERSION 3.5.2)

add_test(NAME cryptor_tests COMMAND tox) 

#set(ENV{PYTHON_PATH} ./)
#set(PYTEST_OPTIONS -v -s --color=yes)
#add_test(NAME current_test COMMAND py.test ${PYTEST_OPTIONS}
#    tests/cipher_test.py
#    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
