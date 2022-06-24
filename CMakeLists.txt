
cmake_minimum_required(VERSION 3.9)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -Wshadow -Wextra -Wformat=2 -Wfloat-equal -Wconversion -Wlogical-op -Wcast-qual -Wcast-align -fstack-protector")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wshadow -Wextra -Wformat=2 -Wfloat-equal -Wconversion -Wlogical-op -Wcast-qual -Wcast-align -fstack-protector -pedantic")

project(multithread-client-server C CXX)

set(UNIT_TARGET unit-app)
set(APP_TARGET test-app)

set(TEST_SRC
    "gtest/UnitApp.cpp"
    "src/Server.cpp"
    )

set(APP_SRC
    "main.cpp"
    "src/Server.cpp"
    "src/Client.cpp"
    )

find_package(Threads REQUIRED)
find_package(GTest REQUIRED)

enable_testing()

set (GTEST_LIB GTest::GTest GTest::Main)

add_executable(${UNIT_TARGET} ${TEST_SRC})
add_executable(${APP_TARGET} ${APP_SRC})

target_link_libraries(${UNIT_TARGET} ${CMAKE_THREAD_LIBS_INIT} ${GTEST_LIB})
target_link_libraries(${APP_TARGET} ${CMAKE_THREAD_LIBS_INIT} )

include_directories(${CMAKE_SOURCE_DIR}/src)