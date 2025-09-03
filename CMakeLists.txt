cmake_minimum_required(VERSION 3.29)
project(fixengine)

set(CMAKE_CXX_STANDARD 20)

find_package(Boost 1.88.0 REQUIRED)

file(GLOB SOURCES ./src/* ./src/*/* ./src/*/*/*)
add_library(${PROJECT_NAME} ${SOURCES})

add_subdirectory(third-party/yaml-cpp)

target_include_directories(${PROJECT_NAME}
        PUBLIC
            $<INSTALL_INTERFACE::include>
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
            ${Boost_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME}
        PUBLIC
            ${Boost_LIBRARIES}
            yaml-cpp
        PRIVATE
            ws2_32
)

add_executable(main main.cpp)
target_link_libraries(main PRIVATE ${PROJECT_NAME})

# Tests
add_executable(msg_builder_test ../test/msg_builder_test.cpp)
target_link_libraries(msg_builder_test PRIVATE ${PROJECT_NAME})

add_executable(session_test ../test/session_test.cpp)
target_link_libraries(session_test PRIVATE ${PROJECT_NAME})

add_executable(yaml_test ../test/yaml_test.cpp)
target_link_libraries(yaml_test PRIVATE ${PROJECT_NAME})
