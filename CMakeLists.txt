cmake_minimum_required(VERSION 3.11.0)

project (csi-hostpath-cpp VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Set all required dependencies path
set(gRPC_DIR "${CMAKE_SOURCE_DIR}/vendor/grpc/lib/cmake/grpc/")
set(protobuf_DIR "${CMAKE_SOURCE_DIR}/vendor/grpc/lib64/cmake/protobuf/")

# Find all required packages
find_package(Threads REQUIRED)
find_package(protobuf CONFIG REQUIRED)
find_package(gRPC CONFIG REQUIRED)

add_executable(hostpath main.cpp)
target_link_libraries(hostpath gRPC::grpc++)
