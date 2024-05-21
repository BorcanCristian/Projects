cmake_minimum_required(VERSION 3.10)
project(PlatformerGame)

set(CMAKE_CXX_STANDARD 17)

add_executable(PlatformerGame WIN32 src/
        "win32_platform.cpp"
)