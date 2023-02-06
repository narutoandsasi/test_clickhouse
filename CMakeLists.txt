set(PROJECT_NAME test_clickhouse)
project(${PROJECT_NAME})
CMAKE_MINIMUM_REQUIRED (VERSION 3.0.2)


include_directories(deps/clickhouse-cpp)
include_directories(deps/clickhouse-cpp/contrib)

add_subdirectory(deps/clickhouse-cpp)
add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} clickhouse-cpp-lib-static)
