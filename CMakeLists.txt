# Copyright (c) 2013 Hugues Luc Bruant <hugues@qcodeedit.org>
# All rights reserved.

cmake_minimum_required(VERSION 2.6)

project(RotiDeCode)

find_package(Qt4 4.6 REQUIRED)
find_package(QCodeEdit 3.0 REQUIRED)

include(${QT_USE_FILE})

add_definitions(-Wall -Wextra)

add_qce_executable(rotidecode
    HDRS
        app.h
        window.h
        buffers.h
        searchproxymodel.h
        commandbar.h
    SRCS
        app.cpp
        window.cpp
        buffers.cpp
        searchproxymodel.cpp
        commandbar.cpp
        main.cpp
)

set(DATA_FILES
    "data/common.qxm"
    "data/common.qxf"
    
    "data/cpp.xsh"
    "data/tex.xsh"
    "data/xml.xsh"
)

install(TARGETS rotidecode DESTINATION bin)
install(FILES ${DATA_FILES} DESTINATION share/rotidecode)
