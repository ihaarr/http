cmake_minimum_required(VERSION 3.14)
project(http)

option(LOG_DEBUG "Write debug logs" OFF)
option(BUILD_WITH_TESTS "Build with tests" OFF)
if (LOG_DEBUG)
    add_definitions(-DHTTP_LOG_DEBUG)
    message("LOG_DEBUG sets to ON. Enabled debug logs")
endif()
if (BUILD_WITH_TESTS)
    message("Building with tests")
    add_subdirectory(tests)
endif()

set(CMAKE_CXX_STANDARD 17)
set(BOOST_ROOT /usr/local/include/boost)
set(BOOST_INCLUDE_DIR /usr/local/include/boost)
set(BOOST_LIBRARYDIR /usr/local/lib/boost)
find_package(Boost REQUIRED)

set(HEADERS 
    include/http/server/server.hpp
    include/http/request/request.hpp
    )
set(SOURCES 
    src/server/server.cpp
    src/request/request.cpp
    )

add_library(${PROJECT_NAME} STATIC ${SOURCES} ${HEADERS}) 
target_include_directories(${PROJECT_NAME} PUBLIC include/ PRIVATE ${BOOST_INCLUDE_DIR})