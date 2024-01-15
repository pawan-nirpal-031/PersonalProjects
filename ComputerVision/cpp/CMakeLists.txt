cmake_minimum_required(VERSION 3.12)

project(MachineVision)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)

# Add source files
set(SOURCES
    src/Utils/Core/Core.h
    src/Utils/Kernals/GaussianKernal.h
    src/BasicTransforms/GaussianTransform.h
    src/ArtifactDetectors/Edges/CannyEdgeDetector.cpp
)

# Find OpenCV (assuming it's used in your project)
find_package(OpenCV REQUIRED)

# Include OpenCV headers
include_directories(${OpenCV_INCLUDE_DIRS})

# Create executable
add_executable(MachineVision ${SOURCES})

# Link OpenCV libraries
target_link_libraries(MachineVision ${OpenCV_LIBS})

# Compiler optimization options
target_compile_options(MachineVision PRIVATE
    -O3    # Optimization level 3
    -march=native    # Generate code for the host machine's architecture
    -Wall    # Enable most warning messages
    # Add any other optimization flags you prefer
)
