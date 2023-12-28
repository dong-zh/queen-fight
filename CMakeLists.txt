cmake_minimum_required(VERSION 3.17)
include(FetchContent)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# Set the project name
project(nutdealer)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Where to find the libraries
file(GLOB CHICKEN3421_SOURCES "src/chicken3421/*.cpp")
file(GLOB STB_CMAKE_SOURCES "src/stb-cmake/*.cpp")
file(GLOB QUEEN_FIGHT_SOURCES "src/queen-fight/*.cpp")

add_executable(nutdealer ${QUEEN_FIGHT_SOURCES} ${CHICKEN3421_SOURCES}
                         ${STB_CMAKE_SOURCES})

# Link everything together
target_link_libraries(nutdealer glfw glad glm tinyobjloader)

# Add external libraries
FetchContent_Declare(
  imgui-glfw-glad-glm
  GIT_REPOSITORY https://github.com/cmmw/imgui-glfw-glad-glm.git
  GIT_TAG master)
FetchContent_Declare(
  tinyobjloader
  GIT_REPOSITORY https://github.com/tinyobjloader/tinyobjloader
  GIT_TAG release)

FetchContent_MakeAvailable(imgui-glfw-glad-glm tinyobjloader)

# Specify include directories
target_include_directories(nutdealer PRIVATE "${PROJECT_BINARY_DIR}"
                                             "${PROJECT_SOURCE_DIR}/include")

# Copy the res directory to the bin directory after the build
add_custom_command(
  TARGET nutdealer
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/res
          $<TARGET_FILE_DIR:${PROJECT_NAME}>/res)
