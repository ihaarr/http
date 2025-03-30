cmake_minimum_required(VERSION 3.18)

function(addtest name sources)
    add_executable(${name} ${sources})
    target_include_directories(${name} PRIVATE ../include)
    target_link_libraries(${name} PUBLIC ${PROJECT_NAME} gtest gtest_main) 
    add_test(NAME ${name} COMMAND ${name})
endfunction()

find_package(GTest REQUIRED)
include_directories( ${GTEST_INCLUDE_DIRS} )
enable_testing()

addtest(request-parser testrequestparser.cpp)
