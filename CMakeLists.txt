cmake_minimum_required(VERSION 3.10)

project(OpenCVTutorial VERSION 1.0 LANGUAGES CXX CUDA)

find_package(OpenCV REQUIRED)
# find_package(OpenMP REQUIRED)
find_package(CUDAToolkit)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# set(CMAKE_CXX_FLAGS "-Wall -Wextra -O3 -march=native -fopenmp")
set(CMAKE_CXX_FLAGS "-Wall -Wextra -O3 -march=native -pthread")

include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(${CUDAToolkit_INCLUDE_DIRS})

message(STATUS "C++ Compiler: ${CMAKE_CXX_COMPILER}")
message(STATUS "C++ Flags: ${CMAKE_CXX_FLAGS}")
message(STATUS "CUDA Compiler: ${CMAKE_CUDA_COMPILER}")

configure_file(Config.h.in Config.h)

file(GLOB_RECURSE SOURCES "src/*.cpp")

foreach(source ${SOURCES})
  get_filename_component(target_name ${source} NAME_WE)
  get_filename_component(dir_name ${source} DIRECTORY)

  string(REPLACE ${CMAKE_SOURCE_DIR}/src ${CMAKE_BINARY_DIR} binary_dir ${dir_name})

  add_executable(${target_name} ${source})

  target_link_libraries(${target_name} ${OpenCV_LIBS} CUDA::cudart)
  # if(OpenMP_CXX_FOUND)
  #     target_link_libraries(${target_name} OpenMP::OpenMP_CXX)
  # endif()

  install(TARGETS ${target_name} DESTINATION bin)
endforeach()

file(WRITE clean-all.cmake
  "file(REMOVE_RECURSE ${CMAKE_BINARY_DIR}/*)\n"
)
