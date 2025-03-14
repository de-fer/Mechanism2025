cmake_minimum_required(VERSION 3.16)

project(Mechanism)

find_package (SDL3 CONFIG REQUIRED)
find_package (flecs CONFIG REQUIRED)

add_executable(${PROJECT_NAME} main.cpp
)

target_sources(${PROJECT_NAME} PRIVATE
    Model.hpp
    Model.cpp
    Node.hpp
    Texture.hpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    SDL3::SDL3
    flecs::flecs
)
