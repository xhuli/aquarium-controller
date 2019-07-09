cmake_minimum_required(VERSION 3.13)

set(CMAKE_BUILD_TYPE Debug)

#set(This UnitTests)
#set(Sources
#        AlarmStationTest/AlarmStationTest.cpp
#        AtoStationTest/AtoStationTest.cpp
#        DosingStationTest/DosingPortTest.cpp
#        TemperatureControlStationTest/TemperatureControlStationTest.cpp
#)
#add_executable(${This} ${Sources})
#add_test(NAME ${This} COMMAND ${Sources})

add_executable(AlarmStationTest AlarmStationTest/AlarmStationTest.cpp)
add_test(NAME AlarmStationTest COMMAND TestAlarmstation)

add_executable(AtoStationTest AtoStationTest/AtoStationTest.cpp)
add_test(NAME AtoStationTest COMMAND TestAtostation)

add_executable(DosingPortTest DosingStationTest/DosingPortTest.cpp)
add_test(NAME DosingPortTest COMMAND TestDosingPort)

add_executable(TemperatureControlStationTest TemperatureControlStationTest/TemperatureControlStationTest.cpp)
add_test(NAME TemperatureControlStationTest COMMAND TestTemperatureControlStation)

