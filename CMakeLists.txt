cmake_minimum_required(VERSION 3.26.4)
project(shovel-sweeper)

set(CMAKE_CXX_STANDARD 17)

set(PROJECT_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include")
set(PROJECT_SRC_DIR "${CMAKE_SOURCE_DIR}/src")

set(SFML_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/SFML-2.6.1/include")
set(SFML_LIBRARY_DIR "${CMAKE_SOURCE_DIR}/SFML-2.6.1/lib")

add_executable(${PROJECT_NAME}
        ${PROJECT_SRC_DIR}/main.cpp
        ${PROJECT_SRC_DIR}/mines.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE
        ${SFML_INCLUDE_DIR}
        ${PROJECT_INCLUDE_DIR}
)

target_link_directories(${PROJECT_NAME} PRIVATE ${SFML_LIBRARY_DIR})

target_link_libraries(${PROJECT_NAME}
        sfml-graphics
        sfml-window
        sfml-system
        opengl32
        winmm
        gdi32
)

message(STATUS "SFML include directory: ${SFML_INCLUDE_DIR}")
message(STATUS "SFML library directory: ${SFML_LIBRARY_DIR}")