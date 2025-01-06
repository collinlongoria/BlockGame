cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

project(BlockGame VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# ------------------------------------------------------------------------------
# Where we want the final .exe to go
# ------------------------------------------------------------------------------
# Use a generator expression to handle multi-config (e.g. Debug/Release) in one go
# This should match wherever you want your exe to land.
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/../build/$<CONFIG>")

# We'll also store it in a variable for our custom commands
set(BIN_DIR "${EXECUTABLE_OUTPUT_PATH}")

# ------------------------------------------------------------------------------
# Gather Sources
# ------------------------------------------------------------------------------
file(GLOB_RECURSE HEADERS_H  "${CMAKE_SOURCE_DIR}/../include/*.h")
file(GLOB_RECURSE HEADERS_HPP "${CMAKE_SOURCE_DIR}/../include/*.hpp")
file(GLOB_RECURSE SOURCES_CPP "${CMAKE_SOURCE_DIR}/../src/*.cpp")
file(GLOB_RECURSE SOURCES_C   "${CMAKE_SOURCE_DIR}/../src/*.c")

set(SOURCES
        ${SOURCES_CPP}
        ${SOURCES_C}
        ${HEADERS_H}
        ${HEADERS_HPP}
)

# ------------------------------------------------------------------------------
# Create Executable
# ------------------------------------------------------------------------------
add_executable(BlockGame ${SOURCES})

target_compile_options(BlockGame PRIVATE -static-libstdc++ -static-libgcc)
target_include_directories(BlockGame PRIVATE "${CMAKE_SOURCE_DIR}/../include")

target_compile_definitions(BlockGame
        PRIVATE
        $<$<CONFIG:Debug>:DEBUG_MODE>
)

# Tell CMake where to place the executable
# (RUNTIME_OUTPUT_DIRECTORY is slightly lower-level than EXECUTABLE_OUTPUT_PATH,
#  but we set both for clarity.)
set_target_properties(BlockGame PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${EXECUTABLE_OUTPUT_PATH}"
)

# ------------------------------------------------------------------------------
# Libraries
# ------------------------------------------------------------------------------
set(LIBRARIES_DIR "${CMAKE_SOURCE_DIR}/../libraries")
set(GLFW_LIB_DEBUG   "${LIBRARIES_DIR}/glfw/glfw3.dll")
set(GLFW_LIB_RELEASE "${LIBRARIES_DIR}/glfw/glfw3.dll")

target_link_libraries(BlockGame PRIVATE
        $<$<CONFIG:Debug>:${GLFW_LIB_DEBUG}>
        $<$<CONFIG:Release>:${GLFW_LIB_RELEASE}>
)

# copy .dll files into build folder
add_custom_command(
        TARGET BlockGame POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<$<CONFIG:Debug>:${GLFW_LIB_DEBUG}>
        $<$<CONFIG:Release>:${GLFW_LIB_RELEASE}>
        "${BIN_DIR}/"
        COMMENT "Copying GLFW DLL to output directory"
)

find_package(OpenGL REQUIRED)
target_link_libraries(BlockGame PRIVATE OpenGL::GL)

# ------------------------------------------------------------------------------
# Resource Copying (Shaders & Assets)
# ------------------------------------------------------------------------------
set(SHADERS_DIR "${CMAKE_SOURCE_DIR}/../shaders")
set(ASSETS_DIR  "${CMAKE_SOURCE_DIR}/../assets")

# 1. Glob all shader/asset files so CMake notices changes.
file(GLOB_RECURSE SHADER_FILES
        "${SHADERS_DIR}/*.frag"
        "${SHADERS_DIR}/*.vert"
        "${SHADERS_DIR}/*.glsl"
        # Add more extensions if needed
)

file(GLOB_RECURSE ASSET_FILES
        "${ASSETS_DIR}/*"
)

# 2. Custom command to remove and re-copy directories; it creates a "stamp" file so
#    we only re-run when the SHADER_FILES or ASSET_FILES actually change.
set(OUTPUT_STAMP "${CMAKE_BINARY_DIR}/copy_resources.stamp")

add_custom_command(
        OUTPUT "${OUTPUT_STAMP}"
        COMMAND ${CMAKE_COMMAND} -E remove_directory
        "${BIN_DIR}/shaders"
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${SHADERS_DIR}"
        "${BIN_DIR}/shaders"

        COMMAND ${CMAKE_COMMAND} -E remove_directory
        "${BIN_DIR}/assets"
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${ASSETS_DIR}"
        "${BIN_DIR}/assets"

        # Touch the stamp so the command doesn't always re-run unless there's a change
        COMMAND ${CMAKE_COMMAND} -E touch "${OUTPUT_STAMP}"

        DEPENDS ${SHADER_FILES} ${ASSET_FILES}
        COMMENT "Removing and copying all shaders and assets..."
)

# 3. A custom target that depends on the stamp file
add_custom_target(copy_resources ALL
        DEPENDS "${OUTPUT_STAMP}"
)

# 4. Ensure that we build resources before building the exe,
#    but do NOT reference the exe's output property from within the same custom command.
add_dependencies(BlockGame copy_resources)

# ------------------------------------------------------------------------------
# Show some info
# ------------------------------------------------------------------------------
message(STATUS "Project:         BlockGame")
message(STATUS "C++ Standard:    ${CMAKE_CXX_STANDARD}")
message(STATUS "Source Files:    ${SOURCES}")
message(STATUS "Shaders Directory: ${SHADERS_DIR}")
message(STATUS "Assets Directory:  ${ASSETS_DIR}")
message(STATUS "BIN_DIR:            ${BIN_DIR}")
