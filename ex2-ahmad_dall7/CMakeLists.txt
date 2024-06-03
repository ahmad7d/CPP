cmake_minimum_required(VERSION 3.21)
project(ex5_ahmad_dall7)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(ex5_ahmad_dall7
        Movie.cpp
        Movie.h
        RecommenderSystem.cpp
        RecommenderSystem.h
        RecommenderSystemLoader.cpp
        RecommenderSystemLoader.h
        RSUser.cpp
        RSUser.h
        RSUsersLoader.cpp
        RSUsersLoader.h
        presubmit.cpp
#        print_main.cpp
        )
