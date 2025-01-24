#!/bin/bash

# Exit the script if any command fails
set -e

# Set the build type, default to Release
BUILD_TYPE=${1:-Release}

# Set the build directory
BUILD_DIR="build"

# If the build directory exists, remove it
if [ -d "$BUILD_DIR" ]; then
  echo "Removing existing build directory..."
  rm -rf "$BUILD_DIR"
fi

# Create a new build directory
mkdir "$BUILD_DIR"

# Navigate into the build directory
cd "$BUILD_DIR"

# Run CMake configuration
cmake -DCMAKE_BUILD_TYPE=$BUILD_TYPE ..

# Compile the project
cmake --build .

# Notify that the build has finished successfully
echo "Build finished successfully. The executable has been copied to the project root directory."
