enable_testing()
add_executable(sini2D_Tests
  "${CMAKE_CURRENT_SOURCE_DIR}/TestConfig.cpp"

  "${CMAKE_CURRENT_SOURCE_DIR}/math/VectorTesting.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/math/MatrixTesting.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/geometry/LineTesting.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/geometry/PolygonTesting.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/gl/CameraTesting.cpp"
  )
target_link_libraries(sini2D_Tests sini2D)
add_test(NAME sini2D_Tests COMMAND sini2D_Tests)
target_compile_options(sini2D_Tests
  PRIVATE
  $<$<CONFIG:Debug>:${PRIVATE_DEBUG_COMPILE_FLAGS}>
  $<$<CONFIG:Release>:${PRIVATE_RELEASE_COMPILE_FLAGS}>
  $<$<CONFIG:Unspecified>:${PRIVATE_UNSPECIFIED_COMPILE_FLAGS}>
)

add_executable(sini2D_ManualTest
  "${CMAKE_CURRENT_SOURCE_DIR}/WindowTesting.cpp"
)
target_link_libraries(sini2D_ManualTest sini2D)
target_compile_options(sini2D_ManualTest
  PRIVATE
  $<$<CONFIG:Debug>:${PRIVATE_DEBUG_COMPILE_FLAGS}>
  $<$<CONFIG:Release>:${PRIVATE_RELEASE_COMPILE_FLAGS}>
  $<$<CONFIG:Unspecified>:${PRIVATE_UNSPECIFIED_COMPILE_FLAGS}>
)

add_executable(sini2D_DrawingBenchmark
  "${CMAKE_CURRENT_SOURCE_DIR}/gl/DrawingBenchmark.cpp")
target_link_libraries(sini2D_DrawingBenchmark sini2D)
target_compile_options(sini2D_DrawingBenchmark
  PRIVATE
  $<$<CONFIG:Debug>:${PRIVATE_DEBUG_COMPILE_FLAGS}>
  $<$<CONFIG:Release>:${PRIVATE_RELEASE_COMPILE_FLAGS}>
  $<$<CONFIG:Unspecified>:${PRIVATE_UNSPECIFIED_COMPILE_FLAGS}>
)

if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  file(COPY dll/SDL2.dll DESTINATION "${CMAKE_BINARY_DIR}/bin/Debug")
  file(COPY dll/SDL2.dll DESTINATION "${CMAKE_BINARY_DIR}/bin/Release")
endif()
