cmake_minimum_required(VERSION 3.10)

project(LSVCS VERSION 1.0)

# C++ Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Header files
include_directories(include)

# Create library
add_library(lsvcslib
    src/Blob.cpp
    src/Branch.cpp
    src/Checkout.cpp
    src/Commit.cpp
    src/Graphviz.cpp
    src/SHA256.cpp
    src/Staging.cpp
    src/Tree.cpp
)

# Executable
add_executable(lsvcs
    src/lsvcs.cpp
)

# Link library
target_link_libraries(lsvcs PRIVATE lsvcslib)

# OpenSSL
find_package(OpenSSL REQUIRED)
target_link_libraries(lsvcslib PUBLIC OpenSSL::Crypto)

# Filesystem (needed only for GCC < 9)
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 9)
        target_link_libraries(lsvcs PRIVATE stdc++fs)
    endif()
endif()