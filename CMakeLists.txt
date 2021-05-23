cmake_minimum_required(VERSION 3.10)

project (HostPathCPP VERSION 0.0.0 LANGUAGES CXX)

configure_file(HostPathCPPConfig.h.in HostPathCPPConfig.h)

set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fpic -O2 -Wall -Wno-unused-but-set-variable  -Wno-misleading-indentation -Wno-deprecated-declarations -Wno-parentheses -Wno-switch -Wno-literal-suffix -Wno-conversion-null -Wno-format-contains-nul")

# Set all required dependencies path
set(gRPC_DIR "${CMAKE_SOURCE_DIR}/vendor/grpc/lib/cmake/grpc/")
set(protobuf_DIR "${CMAKE_SOURCE_DIR}/vendor/grpc/lib/cmake/protobuf/")

# Find all required packages
find_package(Threads REQUIRED)
find_package(protobuf CONFIG REQUIRED)
find_package(gRPC CONFIG REQUIRED)

add_executable(hostpath
		main.cpp
		vendor/loguru/loguru.cpp
		proto/csi.pb.cc
		proto/csi.grpc.pb.cc
		proto/hostpath.pb.cc
		src/state.cpp
		src/csi_services.cpp
		src/identity_services.cpp
		src/controller_services.cpp
		src/node_services.cpp
		src/utils.cpp
		)

target_include_directories(hostpath PRIVATE
				"${CMAKE_SOURCE_DIR}"
				"${CMAKE_SOURCE_DIR}/vendor"
				"${CMAKE_SOURCE_DIR}/src"
				"${CMAKE_SOURCE_DIR}/proto")

target_include_directories(hostpath PUBLIC "${PROJECT_BINARY_DIR}")

target_link_libraries(hostpath gRPC::grpc++ stdc++fs)
