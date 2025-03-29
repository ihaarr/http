cmake_minimum_required(VERSION 3.14)
project(http)

set(BOOST_ROOT /usr/local/include/boost)
set(BOOST_LIBRARYDIR /usr/local/lib/boost)

set(HEADERS 
    include/http/server/server.hpp
    )
set(SOURCES 
    src/main.cpp
    src/server/server.cpp
    )

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS}) 
target_include_directories(${PROJECT_NAME} PUBLIC include/)
