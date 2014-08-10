cmake_minimum_required(VERSION 2.8)
include(FRCDeploy)

project(Team1257Robot)
find_package(WPILib)
include_directories(${WPILib_INCLUDE_DIRS})

file(GLOB_RECURSE SOURCE_CODE *.cpp)

add_executable(FRC_UserProgram.out ${SOURCE_CODE})
target_link_libraries(FRC_UserProgram.out ${WPILib_LIBRARY})

add_make_deploy(FRC_UserProgram.out 10.12.57.2) 
