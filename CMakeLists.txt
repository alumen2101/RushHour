cmake_minimum_required(VERSION 3.16)

project(RushHour 
        VERSION 0.1.0
        DESCRIPTION "Rush Hour"
        LANGUAGES CXX)

# C++23
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Enable Hot Reload for MSVC compilers if supported
if(POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

# output dirs
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# build type
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Choose the type of build" FORCE)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

# ===== RAYLIB SETUP OPTIONS =====

# Option 1: Use pre-compiled Raylib (if available)
# Try to find an installed version of Raylib
find_package(raylib 4.5 QUIET)

# Option 2: Use a specific Raylib directory (if specified)
if(NOT raylib_FOUND)
  set(RAYLIB_DIR "" CACHE PATH "Path to Raylib installation directory")
  if(RAYLIB_DIR)
    message(STATUS "Using Raylib from specified directory: ${RAYLIB_DIR}")
    set(raylib_FOUND TRUE)
    include_directories(${RAYLIB_DIR}/include)
    link_directories(${RAYLIB_DIR}/lib)
  endif()
endif()

# Option 3: Use the modified FetchContent approach (if needed)
if(NOT raylib_FOUND)
  message(STATUS "Raylib not found. Setting up to build from source.")
  
  # Before FetchContent, let's make sure we control what happens
  set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
  set(BUILD_GAMES OFF CACHE BOOL "" FORCE)
  set(SUPPORT_FILEFORMAT_GLTF OFF CACHE BOOL "" FORCE) # Reduce dependencies
  
  # On Windows, force use of MinGW instead of MSVC if using GCC
  if(WIN32 AND CMAKE_C_COMPILER_ID STREQUAL "GNU")
    set(USE_MSVC_RUNTIME_LIBRARY_DLL OFF CACHE BOOL "" FORCE)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -static")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static")
  endif()
  
  include(FetchContent)
  FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG master
    GIT_SHALLOW TRUE
  )
  
  # Custom handling of FetchContent
  FetchContent_GetProperties(raylib)
  if(NOT raylib_POPULATED)
    FetchContent_Populate(raylib)
    
    # Add the raylib directory as a subdirectory of our project
    add_subdirectory(${raylib_SOURCE_DIR} ${raylib_BINARY_DIR})
    
    # Make sure raylib's include directories are accessible
    include_directories(${raylib_SOURCE_DIR}/src)
    
    message(STATUS "Raylib source downloaded and configured successfully.")
  endif()
endif()

# Define source files
# For now, we'll use your original files, but adapt to the new structure as you implement it
set(SOURCES 
    RushHour.cpp
)

set(HEADERS
    RushHour.h
)

# Add executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Set compiler warnings
if(MSVC)
  target_compile_options(${PROJECT_NAME} PRIVATE /W4)
else()
  target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra)
endif()

# Link with Raylib
if(raylib_FOUND AND NOT TARGET raylib)
  # If we found a pre-installed version or specified a directory
  target_link_libraries(${PROJECT_NAME} PRIVATE raylib)
else()
  # If we built it from source using FetchContent
  target_link_libraries(${PROJECT_NAME} PRIVATE raylib)
endif()

# Platform-specific settings
if(WIN32)
  if(MINGW)
    # MinGW-specific options
    target_link_libraries(${PROJECT_NAME} PRIVATE mingw32 opengl32 gdi32 winmm)
  endif()
elseif(APPLE)
  # macOS-specific options
  target_link_libraries(${PROJECT_NAME} PRIVATE "-framework Cocoa" "-framework OpenGL" "-framework IOKit")
elseif(UNIX AND NOT APPLE)
  # Linux-specific options
  target_link_libraries(${PROJECT_NAME} PRIVATE GL m pthread dl rt X11)
endif()

# Install targets
install(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)