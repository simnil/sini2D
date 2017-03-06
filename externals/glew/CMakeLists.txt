add_definitions(-DGLEW_STATIC)

find_package(OpenGL REQUIRED)

# Disable warnings
if(MSVC)
	set(CMAKE_CXX_FLAGS "/W1")
else()
	set(CMAKE_CXX_FLAGS "-w")
endif()


# Compile GLEW
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
add_library(GLEW_2_0_0
		STATIC
		src/glew.c)

target_link_libraries(GLEW_2_0_0
		${OPENGL_LIBRARIES})


set(GLEW_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/include PARENT_SCOPE)
set(GLEW_LIBRARIES GLEW_2_0_0 PARENT_SCOPE)