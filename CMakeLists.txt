cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project("sini2D")

# Require 64-bit compiler
# From sfzCore by PetorSFZ (github.com/PetorSFZ)
if(NOT (CMAKE_SIZEOF_VOID_P EQUAL 8))
	message(FATAL_ERROR "Can only compile in 64-bit mode")
endif()


# ---------------
# Compiler flags
# ---------------
# W4 = Warnings (level 4)
# Zi = Produce .pdb debug information
# wd4201 = Disable warning 4201 (non-standard extension used, nameless struct/union)
# /Od / -O0 = Disable optimization for faster compiling and simpler debugging
if (MSVC)
	# MSVC compiler flags
	set(CMAKE_CXX_FLAGS "/W4 /Zi /std:c++17 /wd4201")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /Od")
else()
	# GCC / CLANG compiler flags
	set(CMAKE_CXX_FLAGS "-Wall -Wextra -std=c++17")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")
endif()


# ------------
# Directories
# ------------
set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(EXTERNALS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/externals)
set(TEST_DIR ${CMAKE_CURRENT_SOURCE_DIR}/tests)
set(CMAKE_MODULES ${CMAKE_CURRENT_LIST_DIR}/cmake)
# Add cmake modules to module path
list(APPEND CMAKE_MODULE_PATH ${CMAKE_MODULES})


# -------------------
# External libraries
# -------------------
# SDL2
find_package(SDL2 REQUIRED)
# GLEW
add_definitions(-DGLEW_STATIC)
add_subdirectory(${EXTERNALS_DIR}/glew)
# OpenGL
find_package(OpenGL REQUIRED)
# Catch
add_subdirectory(${EXTERNALS_DIR}/catch)


# --------------------
# Include directories
# --------------------
include_directories(
	${INCLUDE_DIR}
	${SDL2_INCLUDE_DIRS}
	${GLEW_INCLUDE_DIRS}
	${OPENGL_INCLUDE_DIRS}
	${CATCH_INCLUDE_DIR}
)


# -------------
# Source files
# -------------
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
# Group source files
source_group(sini2D_math FILES ${SINI_2D_MATH_FILES})
source_group(sini2D_util FILES ${SINI_2D_UTIL})
source_group(sini2D_geometry FILES ${SINI_2D_GEOMETRY_FILES})
source_group(sini2D_sdl FILES ${SINI_2D_SDL_FILES})
source_group(sini2D_gl FILES ${SINI_2D_GL_FILES})


# ---------------
# Link libraries
# ---------------
#set_directory_source_group(${SINI_2D_SRC_FILES})
add_library(sini2DLib ${SINI_2D_SRC_FILES})

# Linking libraries
target_link_libraries(
	sini2DLib

	${GLEW_LIBRARIES}
	${SDL2_LIBRARIES}
	${OPENGL_LIBRARIES}
)


# --------
# Testing
# --------
set(SINI_2D_BUILD_TESTS TRUE) # Activate building tests during development
if(SINI_2D_BUILD_TESTS)
	set(SINI_2D_TEST_FILES
		${TEST_DIR}/sini/TestConfig.cpp

		${TEST_DIR}/sini/math/VectorTesting.cpp
		${TEST_DIR}/sini/math/MatrixTesting.cpp
		${TEST_DIR}/sini/geometry/LineTesting.cpp
		${TEST_DIR}/sini/geometry/PolygonTesting.cpp
		${TEST_DIR}/sini/gl/CameraTesting.cpp
	)
	#TODO structure tests into source groups?

	enable_testing(true)
	add_executable(sini2DTests ${SINI_2D_TEST_FILES})
	target_link_libraries(
		sini2DTests

		sini2DLib
	)
	add_test(sini2DTesting sini2DTests)

	# Manual tests -- might be removed in the release
	set(SINI_2D_MANUAL_TEST_FILES
		${TEST_DIR}/sini/WindowTesting.cpp
	)
	add_executable(sini2DManualTests ${SINI_2D_MANUAL_TEST_FILES})
	target_link_libraries(
		sini2DManualTests

		sini2DLib
	)
	if(MSVC)
		file(COPY dll/SDL2.dll DESTINATION ${CMAKE_BINARY_DIR}/Debug)
		file(COPY dll/SDL2.dll DESTINATION ${CMAKE_BINARY_DIR}/Release)
	endif()
	# end temporary
endif()


# -----------------
# Output variables
# -----------------
set(SINI_2D_INCLUDE_DIRS
	${INCLUDE_DIR}
	${SDL2_INCLUDE_DIRS}
	${GLEW_INCLUDE_DIRS}
	${OPENGL_INCLUDE_DIR}
	PARENT_SCOPE
)
set(SINI_2D_LIBRARIES
	sini2DLib
	${SDL2_LIBRARIES}
	${GLEW_LIBRARIES}
	${OPENGL_LIBRARIES}
)
set(SINI_2D_DLLS
	${SDL2_DLLS}
	${GLEW_DLLS}
	PARENT_SCOPE
)
