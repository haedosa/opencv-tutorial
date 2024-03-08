cmake_minimum_required(VERSION 3.10)

project(OpenCVTutorial VERSION 1.0 LANGUAGES CXX)

find_package(OpenCV REQUIRED)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(${OpenCV_INCLUDE_DIRS})

file(GLOB_RECURSE SOURCES "src/*.cpp")

foreach(source ${SOURCES})
  get_filename_component(target_name ${source} NAME_WE)
  get_filename_component(dir_name ${source} DIRECTORY)

  string(REPLACE ${CMAKE_SOURCE_DIR}/src ${CMAKE_BINARY_DIR} binary_dir ${dir_name})

  add_executable(${target_name} ${source})

  target_link_libraries(${target_name} ${OpenCV_LIBS})

  install(TARGETS ${target_name} DESTINATION bin)
endforeach()

file(WRITE clean-all.cmake
  "file(REMOVE_RECURSE ${CMAKE_BINARY_DIR}/*)\n"
)
