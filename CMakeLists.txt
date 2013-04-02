# Copyright (c) 2013 Hugues Luc Bruant. All rights reserved.
# Contact: hugues.bruant@gmail.com
#
# CMake project file for RotiDeCode

cmake_minimum_required(VERSION 2.5)

project(RotiDeCode)

find_package(Qt4 4.6 REQUIRED)
find_package(QCodeEdit 3.0 REQUIRED)

include(${QT_USE_FILE})

add_definitions(-Wall -Wextra)

add_qce_executable(rotidecode
    HDRS
        app.h
        window.h
        editor.h
        buffers.h
    SRCS
        app.cpp
        window.cpp
        editor.cpp
        buffers.cpp
        main.cpp
)
