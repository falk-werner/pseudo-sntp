cmake_minimum_required(VERSION 3.10)
project(pseude-sntp VERSION 1.0.0 DESCRIPTION "Testing SNTP")

option(WITHOUT_EXAMPLES "Do not build examples" OFF)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-Wall -Wextra -Wpedantic)

add_library(pseudo_sntp STATIC
    src/pseudo_sntp/resolve.cpp
    src/pseudo_sntp/endpoint.cpp
    src/pseudo_sntp/udp_client.cpp
    src/pseudo_sntp/udp_server.cpp
    src/pseudo_sntp/sntp_client.cpp
    src/pseudo_sntp/sntp_server.cpp
    src/pseudo_sntp/sntp_packet.cpp
    src/pseudo_sntp/sntp_timestamp.cpp
)

target_include_directories(pseudo_sntp PUBLIC include)
target_include_directories(pseudo_sntp PRIVATE src)

if(NOT WITHOUT_EXAMPLES)

add_executable(sntp-client example/client.cpp)
target_link_libraries(sntp-client PRIVATE pseudo_sntp)

add_executable(sntp-server example/server.cpp)
target_link_libraries(sntp-server PRIVATE pseudo_sntp)

endif()