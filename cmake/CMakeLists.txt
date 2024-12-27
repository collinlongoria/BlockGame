# CMake version
cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

# Project name and version
project(BlockGame VERSION 0.1.0)

# C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# include directories
include_directories("${CMAKE_SOURCE_DIR}/../include")
file(GLOB_RECURSE HEADERS_H "${CMAKE_SOURCE_DIR}/../include/*.h")
file(GLOB_RECURSE HEADERS_HPP "${CMAKE_SOURCE_DIR}/../include/*.hpp")

# Gather C++ and C from src directory
file(GLOB_RECURSE SOURCES_CPP "${CMAKE_SOURCE_DIR}/../src/*.cpp")
file(GLOB_RECURSE SOURCES_C "${CMAKE_SOURCE_DIR}/../src/*.c")

# Combine source files into list
set(SOURCES ${SOURCES_CPP} ${SOURCES_C} ${HEADERS_H} ${HEADERS_HPP})

# Executable target
add_executable(BlockGame ${SOURCES})

# Define DEBUG_MODE for Debug builds
target_compile_definitions(BlockGame
        PRIVATE
        $<$<CONFIG:Debug>:DEBUG_MODE>
)

# Binary output directory
set_target_properties(BlockGame PROPERTIES RUNTIME_OUTPUT_DIRECTORY $<1:${CMAKE_SOURCE_DIR}/../build/$<CONFIG>>)

# Libraries directory
set(LIBRARIES_DIR "${CMAKE_SOURCE_DIR}/../libraries")

# Set path to glfw
# doing it separate even when it doesn't matter, because sometimes it does
# and I want to keep it consistent
set(GLFW_LIB_DEBUG "${LIBRARIES_DIR}/glfw/glfw3.dll")
set(GLFW_LIB_RELEASE "${LIBRARIES_DIR}/glfw/glfw3.dll")

# Link GLFW
target_link_libraries(BlockGame PRIVATE
        $<$<CONFIG:Debug>:${GLFW_LIB_DEBUG}>
        $<$<CONFIG:Release>:${GLFW_LIB_RELEASE}>
)

# Find OpenGL
find_package(OpenGL REQUIRED)

# Link OpenGL
target_link_libraries(BlockGame PRIVATE OpenGL::GL)

# Set shaders directory
set(SHADERS_DIR "${CMAKE_SOURCE_DIR}/../shaders")

# Copy necessary runtime files after build
add_custom_command(TARGET BlockGame POST_BUILD
        # Copy GLFW DLL based on configuration
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<$<CONFIG:Debug>:${GLFW_LIB_DEBUG}>
        $<$<CONFIG:Release>:${GLFW_LIB_RELEASE}>
        $<TARGET_FILE_DIR:BlockGame>

        # Copy shaders folder
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${SHADERS_DIR}"
        "$<TARGET_FILE_DIR:BlockGame>/shaders"
)

# Display configuration summary
message(STATUS "Project: BlockGame")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Source Files: ${SOURCES}")
message(STATUS "Libraries Directory: ${LIBRARIES_DIR}")
message(STATUS "Shaders Directory: ${SHADERS_DIR}")
