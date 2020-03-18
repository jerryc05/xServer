cmake_minimum_required(VERSION 2.4)
project(xServer)
message("")

add_executable(xServer
        main.cpp
        x-server/env-settings.hpp
        x-server/Epoll.cpp x-server/Epoll.hpp
        x-server/Socket.cpp x-server/Socket.hpp
        x-server/IpAddr.cpp x-server/IpAddr.hpp
        utils/logger.cpp utils/logger.hpp)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    message("Adding GCC Flags...")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} \
-std=gnu++2a \
-ftrapv \
-Waggregate-return \
-Wall \
-Wcast-align \
-Wconversion \
-Wduplicated-branches \
-Wduplicated-cond \
-Wdouble-promotion \
-Wextra \
-Wfloat-equal \
-Winit-self \
-Wlogical-op \
-Wmisleading-indentation \
-Wnon-virtual-dtor \
-Wnull-dereference \
-Wold-style-cast \
-Woverloaded-virtual \
-Wpedantic \
-Wpointer-arith \
-Wshadow \
-Wsign-conversion \
-Wundef \
-Wunused \
-Wuseless-cast \
-Wwrite-strings")
endif ()
message("")

if (CMAKE_BUILD_TYPE MATCHES "Debug")
    message("CMAKE IN DEBUG MODE")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Og")
elseif (CMAKE_BUILD_TYPE MATCHES "Release")
    message("CMAKE IN RELEASE MODE")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Ofast")
endif ()
message("")

message("[${CMAKE_CXX_FLAGS}]")
message("")
