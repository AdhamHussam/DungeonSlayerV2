cmake_minimum_required(VERSION 3.26)
project(DungeonSlayer)

set(CMAKE_CXX_STANDARD 20) # Adjust as needed

set(SOURCE_FILES DungeonSlayer.cpp) # Assuming your main file is DungeonSlayer.cpp

add_executable(DungeonSlayer DungeonSlayer.cpp)

# Include and link SFML
set(SFML_DIR "${CMAKE_SOURCE_DIR}/SFML-2.6.1-windows-vc17-64-bit/SFML-2.6.1/lib/cmake/SFML")
find_package(SFML 2.6 COMPONENTS graphics audio REQUIRED)
target_link_libraries(DungeonSlayer sfml-graphics sfml-audio)
