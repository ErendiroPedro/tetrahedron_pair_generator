cmake_minimum_required(VERSION 3.1)
project(TetrahedronPairGenerator)

set(CMAKE_CXX_STANDARD 17)

find_package(CGAL REQUIRED)

include_directories(${CMAKE_SOURCE_DIR}/headers)

set(SOURCE_FILES
    src/CSVWriter.cpp
    src/GeometryUtils.cpp
    src/TetrahedronFactory.cpp
    main.cpp
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} CGAL::CGAL)
