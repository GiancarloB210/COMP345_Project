cmake_minimum_required(VERSION 3.29)
project(COMP345_Project)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(COMP345_Project
    Map.cpp
    Map.h
    MapDriver.cpp
    MapDriver.h
        Cards.cpp
        Cards.h
        CardsDriver.cpp
        CardsDriver.h
        Orders.cpp
        OrdersDriver.cpp
        Player.cpp
        PlayerDriver.h
        PlayerDriver.cpp
        MainDriver.cpp
        MainDriver.h
        GameEngine.cpp
        GameEngineDriver.cpp)


