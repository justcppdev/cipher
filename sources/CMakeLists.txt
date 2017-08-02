cmake_minimum_required(VERSION 3.5.2)

add_library(
    ${CMAKE_PROJECT_NAME}_lib
    wipe.cpp
    wipe.hpp
    password.cpp
    password.hpp
    cipher.cpp
    cipher.hpp
    store.hpp
    store.cpp
    store_v0.0.2.cpp
    ${HEADERS})


add_executable(${CMAKE_PROJECT_NAME}
    main.cpp
    ${HEADERS})

target_link_libraries(
    ${CMAKE_PROJECT_NAME}
    ${CMAKE_PROJECT_NAME}_lib
    ${BOOST}
    ${OPENSSL})
