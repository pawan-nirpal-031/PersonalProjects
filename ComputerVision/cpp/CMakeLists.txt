cmake_minimum_required(VERSION 3.12)

project(MachineVision)

if(YOUR_COMPILER_PATH)
    set(CMAKE_CXX_COMPILER ${YOUR_COMPILER_PATH})
endif()

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)

# Add source files
set(SOURCES
    src/Utils/Core/Core.h
    src/Utils/Kernals/GaussianKernal.h
    src/BasicTransforms/GaussianTransform.h
    src/ArtifactDetectors/Edges/CannyEdgeDetector.h
    src/ArtifactDetectors/Corners/HarrisCornerDetector.h
    src/BasicTransforms/MiscTransforms.h
    src/ArtifactDetectors/FeatureDiscriptors/ScaleInvarientFeatureTransform.h
    src/Driver/Driver.cpp
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
    # Add any other optimization flags you prefer
    #-Rpass=slp-vectorizer
)
