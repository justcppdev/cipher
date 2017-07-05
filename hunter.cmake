set(Boost_USE_STATIC_LIBS   ON)
set(Boost_USE_MULTITHREADED ON)
set(BOOST_COMPONENTS
    filesystem
    system
    program_options
    iostreams)

hunter_add_package(Boost COMPONENTS ${BOOST_COMPONENTS})
find_package(Boost CONFIG REQUIRED  ${BOOST_COMPONENTS})
string(REGEX REPLACE "([^;]+)" "Boost::\\1"
    BOOST
 "${BOOST_COMPONENTS}")

hunter_add_package(OpenSSL)
find_package(OpenSSL REQUIRED)
include_directories("${OPENSSL_INCLUDE_DIR}")
set(OPENSSL "${OPENSSL_LIBRARIES}")
