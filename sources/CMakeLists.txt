cmake_minimum_required(VERSION 3.5.2)

add_executable(${CMAKE_PROJECT_NAME}
    main.cpp
    encrypt.cpp
    decrypt.cpp
    wipe.cpp
    encryptor.cpp
    encryptor.hpp
    decryptor.cpp
    decryptor.hpp
    ${HEADERS})

target_link_libraries(${CMAKE_PROJECT_NAME} ${BOOST})
