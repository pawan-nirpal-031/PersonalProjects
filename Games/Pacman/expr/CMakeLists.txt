# CMakeLists.txt

cmake_minimum_required(VERSION 3.12)
project(GameProject)

# Add the source files
set(SOURCES
    utils.h
    board.h
    ghost.h
    pellet.h
    pacman.h
    state.h
    game.cpp  # Assuming this is your main file
)

# Create the executable
add_executable(game ${SOURCES})

# Include directories
target_include_directories(game PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Set C++ standard
set_property(TARGET game PROPERTY CXX_STANDARD 14)

# Specify dependencies if needed (e.g., external libraries)
# target_link_libraries(game PUBLIC some_library)

# Generate compile_commands.json for use with tools like clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
