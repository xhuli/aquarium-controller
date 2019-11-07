cmake_minimum_required(VERSION 3.13)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_BUILD_TYPE Debug)

add_executable(AmbientStationTest AmbientStationTests/AmbientStationTest.cpp)
add_test(NAME AmbientStationTest COMMAND TestAmbientStation)

add_executable(AlarmStationTest AlarmStationTest/AlarmStationTest.cpp)
add_test(NAME AlarmStationTest COMMAND TestAlarmStation)

add_executable(AlarmListTest AlarmStationTest/AlarmListTest.cpp)
add_test(NAME AlarmListTest COMMAND TestAlarmList)

add_executable(AlarmArrayTest AlarmStationTest/AlarmArrayTest.cpp)
add_test(NAME AlarmArrayTest COMMAND TestAlarmArray)

add_executable(LinkedMapTest Common/LinkedMapTest.cpp)
add_test(NAME LinkedMapTest COMMAND TestLinkedMap)

add_executable(LinkedListTest Common/LinkedListTest.cpp)
add_test(NAME LinkedListTest COMMAND TestLinkedList)

add_executable(PushButtonTest Common/PushButtonTest.cpp)
add_test(NAME PushButtonTest COMMAND TestPushButton)

add_executable(AtoStationTest AtoStationTest/AtoStationTest.cpp)
add_test(NAME AtoStationTest COMMAND TestAtostation)

add_executable(AtoStationStateObserverTest AtoStationTest/AtoStationStateObserverTest.cpp)
add_test(NAME AtoStationStateObserverTest COMMAND AtoStationStateObserverTest)

add_executable(DosingPortTest DosingStationTest/DosingPortTest.cpp)
add_test(NAME DosingPortTest COMMAND TestDosingPort)
