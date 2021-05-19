cmake_minimum_required(VERSION 3.11.0)

project (csi-hostpath-cpp VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_STANDARD 17)

# Set all required dependencies path
set(gRPC_DIR "${CMAKE_SOURCE_DIR}/vendor/grpc/lib/cmake/grpc/")
set(protobuf_DIR "${CMAKE_SOURCE_DIR}/vendor/grpc/lib/cmake/protobuf/")

# Find all required packages
find_package(Threads REQUIRED)
find_package(protobuf CONFIG REQUIRED)
find_package(gRPC CONFIG REQUIRED)

add_executable(hostpath
		main.cpp
		proto/csi.pb.cc
		proto/csi.grpc.pb.cc
		src/csi_services.cpp
		src/identity_services.cpp
		src/controller_services.cpp
		src/node_services.cpp
		src/utils.cpp
		)

target_include_directories(hostpath PRIVATE
				"${CMAKE_SOURCE_DIR}"
				"${CMAKE_SOURCE_DIR}/src"
				"${CMAKE_SOURCE_DIR}/proto")

target_link_libraries(hostpath gRPC::grpc++)
