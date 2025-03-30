#!/bin/bash

COMMAND=$1

build() {
    if [ ! -d ".build" ]; then
        mkdir .build
    fi
    FLAGS=$1
    cd .build
    cmake .. $FLAGS 
    cmake --build .
}

case $COMMAND in
    "build")
        build
        ;;
    "tests")
        FLAGS="-DLOG_DEBUG=ON -DBUILD_WITH_TESTS=ON" 
        if build $FLAGS; then
        cd tests
        ctest --output-on-failure 
        fi

        ;;
esac

