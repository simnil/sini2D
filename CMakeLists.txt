cmake_minimum_required(VERSION 3.11)
project("sini2D"
  LANGUAGES CXX
  VERSION 0.2
)


if(NOT CMAKE_SIZEOF_VOID_P EQUAL 8)
  message(WARNING
    "sini2D only supports and is tested in 64-bit mode. "
    "Proceed at own risk.")
endif()

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(WARNING
    "No build type specified. Please set CMAKE_BUILD_TYPE "
    "if a specific build is targeted (Release or Debug).")
  set(CMAKE_BUILD_TYPE "Unspecified")
else()
  message(STATUS "Building in ${CMAKE_BUILD_TYPE} mode")
endif()


# ----------------------------------------------------------
# Project directories
# ----------------------------------------------------------
set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(TEST_DIR ${CMAKE_CURRENT_SOURCE_DIR}/tests)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# Build output
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)


# ----------------------------------------------------------
# Dependencies
# ----------------------------------------------------------
find_package(SDL2 REQUIRED)
find_package(GLEW REQUIRED)

set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/extern/catch2)


# ----------------------------------------------------------
# Source files
# ----------------------------------------------------------
set(SINI_2D_MATH_FILES
  ${INCLUDE_DIR}/sini/math/Vector.hpp
  ${INCLUDE_DIR}/sini/math/Vector.inl
  ${INCLUDE_DIR}/sini/math/Matrix.hpp
  ${INCLUDE_DIR}/sini/math/Matrix.inl
  ${INCLUDE_DIR}/sini/math/MathUtilities.hpp
  ${INCLUDE_DIR}/sini/math/MathUtilities.inl
  ${INCLUDE_DIR}/sini/math/MathUtilitiesBase.hpp
  ${INCLUDE_DIR}/sini/math/MathUtilitiesBase.inl
  ${INCLUDE_DIR}/sini/math/VectorUtilities.hpp
  ${INCLUDE_DIR}/sini/math/VectorUtilities.inl
  ${INCLUDE_DIR}/sini/math/MatrixUtilities.hpp
  ${INCLUDE_DIR}/sini/math/MatrixUtilities.hpp
)
set(SINI_2D_UTIL
  ${INCLUDE_DIR}/sini/util/IO.hpp
  ${SOURCE_DIR}/sini/util/IO.cpp
  ${INCLUDE_DIR}/sini/util/testutil.hpp
  ${INCLUDE_DIR}/sini/util/testutil.inl
  ${SOURCE_DIR}/sini/util/testutil.cpp
)
set(SINI_2D_GEOMETRY_FILES
  ${INCLUDE_DIR}/sini/geometry/Line.hpp
  ${INCLUDE_DIR}/sini/geometry/Line.inl
  ${INCLUDE_DIR}/sini/geometry/Polygon.hpp
  ${SOURCE_DIR}/sini/geometry/Polygon.cpp
)
set(SINI_2D_SDL_FILES
  ${INCLUDE_DIR}/sini/sdl/Window.hpp
  ${SOURCE_DIR}/sini/sdl/Window.cpp
  ${INCLUDE_DIR}/sini/sdl/SubsystemInitializer.hpp
  ${SOURCE_DIR}/sini/sdl/SubsystemInitializer.cpp
)
set(SINI_2D_GL_FILES
  ${INCLUDE_DIR}/sini/gl/GLContext.hpp
  ${SOURCE_DIR}/sini/gl/GLContext.cpp
  ${INCLUDE_DIR}/sini/gl/glutil.hpp
  ${SOURCE_DIR}/sini/gl/glutil.cpp
  ${INCLUDE_DIR}/sini/gl/Camera.hpp
  ${SOURCE_DIR}/sini/gl/Camera.cpp
  ${INCLUDE_DIR}/sini/gl/SimpleRenderer.hpp
  ${SOURCE_DIR}/sini/gl/SimpleRenderer.cpp
)
set(SINI_2D_SRC_FILES
  ${INCLUDE_DIR}/sini/CudaCompat.hpp

  ${SINI_2D_MATH_FILES}
  ${SINI_2D_UTIL}
  ${SINI_2D_GEOMETRY_FILES}
  ${SINI_2D_SDL_FILES}
  ${SINI_2D_GL_FILES}
)

# Group source files for Visual Studio
source_group(sini2D_math FILES ${SINI_2D_MATH_FILES})
source_group(sini2D_util FILES ${SINI_2D_UTIL})
source_group(sini2D_geometry FILES ${SINI_2D_GEOMETRY_FILES})
source_group(sini2D_sdl FILES ${SINI_2D_SDL_FILES})
source_group(sini2D_gl FILES ${SINI_2D_GL_FILES})


# ----------------------------------------------------------
# Library
# ----------------------------------------------------------
add_library(sini2D ${SINI_2D_SRC_FILES})
target_include_directories(sini2D
  PUBLIC
    ${INCLUDE_DIR}
    ${SDL2_INCLUDE_DIR}
    ${CATCH_INCLUDE_DIR}
)
target_link_libraries(sini2D
  ${SDL2_LIBRARY}
  GLEW::GLEW
  OpenGL::GL
)


# ----------------------------------------------------------
# Compiler flags
# ----------------------------------------------------------
if(COMPILER_CXX_COMPILER_ID STREQUAL "MSVC")
  # W4 = Warnings (level 4)
  # Zi = Produce .pdb debug information
  # wd4201 = Disable warning 4201 (non-standard extension used, nameless struct/union)
  # Od = Disable optimization
  set(PRIVATE_COMPILE_FLAGS /W4 /Zi /wd4201)
  set(PRIVATE_DEBUG_COMPILE_FLAGS ${PRIVATE_COMPILE_FLAGS} /Od)
  set(PRIVATE_RELEASE_COMPILE_FLAGS ${PRIVATE_COMPILE_FLAGS})
  set(PRIVATE_UNSPECIFIED_COMPILE_FLAGS ${PRIVATE_COMPILE_FLAGS})
elseif(COMPILER_CXX_COMPILER_ID STREQUAL "GNU"
       OR COMPILER_CXX_COMPILER_ID STREQUAL "Clang")
  set(PRIVATE_COMPILE_FLAGS -Wall -Wextra)
  set(PRIVATE_DEBUG_COMPILE_FLAGS ${PRIVATE_COMPILE_FLAGS} -O0)
  set(PRIVATE_RELEASE_COMPILE_FLAGS ${PRIVATE_COMPILE_FLAGS})
  set(PRIVATE_UNSPECIFIED_COMPILE_FLAGS ${PRIVATE_COMPILE_FLAGS} -O2 -g)
endif()

target_compile_features(sini2D PUBLIC cxx_std_17)
target_compile_options(sini2D
  PRIVATE
    $<$<CONFIG:Debug>:${PRIVATE_DEBUG_COMPILE_FLAGS}>
    $<$<CONFIG:Release>:${PRIVATE_RELEASE_COMPILE_FLAGS}>
    $<$<CONFIG:Unspecified>:${PRIVATE_UNSPECIFIED_COMPILE_FLAGS}>
)


# ----------------------------------------------------------
# Tests
# ----------------------------------------------------------
if(SINI_2D_BUILD_TESTS)
  add_executable(sini2DTests
    ${TEST_DIR}/sini/TestConfig.cpp

    ${TEST_DIR}/sini/math/VectorTesting.cpp
    ${TEST_DIR}/sini/math/MatrixTesting.cpp
    ${TEST_DIR}/sini/geometry/LineTesting.cpp
    ${TEST_DIR}/sini/geometry/PolygonTesting.cpp
    ${TEST_DIR}/sini/gl/CameraTesting.cpp
  )
  target_link_libraries(sini2DTests sini2D)

  # Manual tests -- might be removed in the release
  add_executable(sini2DManualTests
    ${TEST_DIR}/sini/WindowTesting.cpp
  )
  target_link_libraries(sini2DManualTests sini2D)

  if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    file(COPY dll/SDL2.dll DESTINATION ${CMAKE_BINARY_DIR}/Debug)
    file(COPY dll/SDL2.dll DESTINATION ${CMAKE_BINARY_DIR}/Release)
  endif()
endif()
