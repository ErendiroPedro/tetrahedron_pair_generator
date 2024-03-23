cmake_minimum_required(VERSION 3.14)
project(TetrahedronPairGenerator)

set(CMAKE_CXX_STANDARD 17)

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
    src/CSVWriter.cpp
    src/JSONWriter.cpp
    src/GeometryUtils.cpp
    src/TetrahedronFactory.cpp
    main.cpp
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} CGAL::CGAL nlohmann_json::nlohmann_json)
