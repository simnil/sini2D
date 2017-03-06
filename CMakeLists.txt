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
# O2 = Optimize for fastest speed
# Od = Disable optimization for faster compiling and simpler debugging
if (MSVC)
	# MSVC compiler flags
	set(CMAKE_CXX_FLAGS "/W4 /Zi /wd4201")
	set(CMAKE_CXX_FLAGS_RELEASE "/02 /NDEBUG")
	set(CMAKE_CXX_FLAGS_DEBUG "/Od /DEBUG")
else()
	# GCC / CLANG compiler flags
	set(CMAKE_CXX_FLAGS "-Wall -Wextra -Zi -std=c++14 -pthread")
	set(CMAKE_CXX_FLAGS_RELEASE "-O3")
	set(CMAKE_CXX_FLAGS_DEBUG "-O0")
endif()


# ------------
# Directories
# ------------
set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(EXTERNALS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/externals)
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


# --------------------
# Include directories
# --------------------
include_directories(
	${SRC_DIR})
	${INCLUDE_DIR}
	${SDL_INCLUDE_DIR}
	${GLEW_INCLUDE_DIR}
	${OPENGL_INCLUDE_DIRS}
)

# -------------
# Source files
# -------------
set(SINI_2D_SRC_FILES
	${INCLUDE_DIR}/sini/CudaCompat.h

	${INCLUDE_DIR}/sini/math/Vector.h
	${INCLUDE_DIR}/sini/math/Vector-defs.h
	${INCLUDE_DIR}/sini/math/Matrix.h
	${INCLUDE_DIR}/sini/math/Matrix-defs.h

	${INCLUDE_DIR}/sini/gl/Window.h
		${SRC_DIR}/sini/gl/Window.cpp
)


# ---------------
# Link libraries
# ---------------
set_directory_source_group(${SINI_2D_SRC_FILES})
add_library(sini2DLib ${SINI_2D_SRC_FILES})

# Linking libraries
target_link_libraries(
	sini2DLib

	${GLEW_LIBRARIES}
	${SDL2_LIBRARIES}
)


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