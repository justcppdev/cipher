cmake_minimum_required(VERSION 3.5.2)

add_executable(${CMAKE_PROJECT_NAME}
    main.cpp
    wipe.cpp
    password.cpp
    cipher.cpp
    ${HEADERS})

target_link_libraries(${CMAKE_PROJECT_NAME} ${BOOST} ${OPENSSL})
