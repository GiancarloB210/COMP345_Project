cmake_minimum_required(VERSION 3.29)
project(COMP345_Project)

set(CMAKE_CXX_STANDARD 17)

include_directories(. MapFiles)
include_directories(. CommandFiles)

file(COPY CommandFiles/command.txt DESTINATION ${CMAKE_BINARY_DIR})
file(COPY MapFiles/USA.map DESTINATION ${CMAKE_BINARY_DIR})
file(COPY MapFiles/USA_Fail1.map DESTINATION ${CMAKE_BINARY_DIR})
file(COPY MapFiles/USA_Fail2.map DESTINATION ${CMAKE_BINARY_DIR})
file(COPY MapFiles/USA_Fail3.map DESTINATION ${CMAKE_BINARY_DIR})

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
        GameEngineDriver.cpp
        CommandProcessing.cpp
        CommandProcessing.h
        ArmyUnit.cpp
        ArmyUnit.h
        CommandProcessingDriver.cpp
        CommandProcessingDriver.h
        Subject.cpp
        LogObserver.h
        LogObserver.cpp
        PlayerStrategies.cpp
        PlayerStrategies.h
        PlayerStrategiesDriver.cpp
        PlayerStrategiesDriver.h
        TournamentDriver.cpp
        TournamentDriver.h)