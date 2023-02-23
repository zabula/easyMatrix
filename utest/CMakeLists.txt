cmake_minimum_required(VERSION 2.8)

project(easyMatrix)
find_package(GTest REQUIRED)

add_executable(main main.cpp)
target_link_libraries(main
    ${GTEST_LIBRARIES}
    ${GTEST_MAIN_LIBRARIES}
    )


