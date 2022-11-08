cmake_minimum_required(VERSION 3.24.2)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
project(chip8)

find_package(SDL2 REQUIRED)

include_directories(${PROJECT_NAME} 
    ${SDL2_INCLUDE_DIRS}
)

add_executable(${PROJECT_NAME} main.cpp Chip8.cpp Platform.cpp)

target_link_libraries(${PROJECT_NAME} 
    ${SDL2_LIBRARIES}
)