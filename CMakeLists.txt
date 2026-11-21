cmake_minimum_required(VERSION 3.16)
project(chip-8_sfml LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(SFML 3 REQUIRED COMPONENTS Graphics Window System Audio)

add_executable(chip-8_sfml 
    src/main.cpp
)

target_include_directories(chip-8_sfml PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(chip-8_sfml PRIVATE
    SFML::Graphics
    SFML::Window
    SFML::System
    SFML::Audio
)

target_compile_options(chip-8_sfml PRIVATE
    -Wall
    -Wextra
    -Wpedantic
)
