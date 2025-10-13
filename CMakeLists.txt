cmake_minimum_required(VERSION 3.10)
project(OceanDepth C)

set(CMAKE_C_STANDARD 99)

include_directories(include)

add_executable(oceandepths
    src/main.c
    src/plongeur.c
    src/creatures.c
    src/combat.c
)
