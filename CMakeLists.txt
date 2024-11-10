cmake_minimum_required(VERSION 3.14)
project(TetrahedronPairGenerator)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)

include(FetchContent)
FetchContent_Declare(
  json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG v3.10.5
)
FetchContent_MakeAvailable(json)

find_package(CGAL REQUIRED)

include_directories(${CMAKE_SOURCE_DIR}/headers)

set(SOURCE_FILES
    src/BaseWriter.cpp
    src/OBJWriter.cpp
    src/CSVWriter.cpp
    src/JSONWriter.cpp
    src/GeometryUtils.cpp
    src/TetrahedronFactory.cpp
    src/Config.cpp 
    main.cpp
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} CGAL::CGAL nlohmann_json::nlohmann_json)
