cmake_minimum_required(VERSION 3.16)

#Директории исходных файлов
set (Mechanism_HEADERS_DIR "include/Mechanism2025")
file(GLOB_RECURSE Mechanism_HEADERS_FILES "${Mechanism_HEADERS_DIR}/*.hpp" "${Mechanism_HEADERS_DIR}/*.h")
file(GLOB_RECURSE Mechanism_SRC_FILES "src/*.cpp")

#Проект Mechanism
project(Mechanism)
add_executable(${PROJECT_NAME})

target_sources(${PROJECT_NAME} PRIVATE ${Mechanism_SRC_FILES} ${Mechanism_HEADERS_FILES})
target_include_directories(${PROJECT_NAME} PRIVATE ${Mechanism_HEADERS_DIR})

#Сторонние пакеты
find_package (SDL3 CONFIG REQUIRED)
find_package (SDL3_image CONFIG REQUIRED)
find_package (nlohmann_json REQUIRED)

target_link_libraries(${PROJECT_NAME} PRIVATE
    SDL3::SDL3
    SDL3_image::SDL3_image-shared
    nlohmann_json::nlohmann_json
)

# Копируем папку assets и configs  в папку сборки
file(COPY ${CMAKE_SOURCE_DIR}/assets DESTINATION ${CMAKE_BINARY_DIR})
file(REMOVE_RECURSE ${CMAKE_BINARY_DIR}/configs)
file(COPY ${CMAKE_SOURCE_DIR}/configs DESTINATION ${CMAKE_BINARY_DIR})
