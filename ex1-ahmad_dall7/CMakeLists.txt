cmake_minimum_required(VERSION 3.21)
project(ex4_ahmad_dall7)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(ex4_ahmad_dall7
#        main.cpp
        Activation.h
        Dense.h
        Matrix.h
        Matrix.cpp
        MlpNetwork.h
        Matrix.cpp
        Dense.cpp
        Activation.cpp
        MlpNetwork.cpp
        presubmit.cpp
        )
