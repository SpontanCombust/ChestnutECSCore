cmake_minimum_required(VERSION 3.0.0)
project(ChestnutECSCore VERSION 0.5.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)


## ======================================== ECS Core ========================================

FetchContent_Declare(
    typelist
    GIT_REPOSITORY https://github.com/SpontanCombust/cpp-typelist
    GIT_TAG master
)

FetchContent_MakeAvailable(typelist)


add_library(${PROJECT_NAME} STATIC)

add_subdirectory(src/chestnut/ecs)

set_property(TARGET ${PROJECT_NAME} PROPERTY POSITION_INDEPENDENT_CODE ON)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(${PROJECT_NAME} typelist)


install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/ DESTINATION include)
install(TARGETS ${PROJECT_NAME} DESTINATION lib)



## ========================================== Tests ========================================

FetchContent_Declare(
    catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2
    GIT_TAG v2.13.7
)

FetchContent_MakeAvailable(catch2)
list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/contrib)

find_package(Threads)


add_executable(${PROJECT_NAME}_Test)

add_subdirectory(tests)

target_link_libraries(${PROJECT_NAME}_Test ${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME}_Test Catch2::Catch2 Threads::Threads)

include(CTest)
include(Catch)
catch_discover_tests(${PROJECT_NAME}_Test)
