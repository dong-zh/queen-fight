cmake_minimum_required(VERSION 3.12)

project("COMP3421 Assignment 1")

set(CMAKE_CXX_STANDARD 17)

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

# If I was better at CMake I would make this nicer
set(REQUIRED_LIBS glad glfw glm stb chicken3421)
set(LIB_DIR ${PROJECT_SOURCE_DIR}/lib)
set(N_DEPS 0)
foreach(LIB ${REQUIRED_LIBS})
    if(EXISTS ${LIB_DIR}/${LIB})
        math(EXPR N_DEPS "${N_DEPS}+1")
    endif()
endforeach()
if (N_DEPS LESS 5)
    set(FETCH_DEP_SCRIPT ${LIB_DIR}/fetch_deps.sh)

    # Configure the "lib" directory as a separate project (whose sole purpose is to fetch dependencies)
    execute_process(COMMAND sh ${FETCH_DEP_SCRIPT} ${CMAKE_GENERATOR} WORKING_DIRECTORY ${LIB_DIR})
endif()

find_package(glad REQUIRED HINTS ${PROJECT_SOURCE_DIR}/lib/glad)
find_package(glfw3 REQUIRED HINTS ${PROJECT_SOURCE_DIR}/lib/glfw)
find_package(glm REQUIRED HINTS ${PROJECT_SOURCE_DIR}/lib/glm)
find_package(stb REQUIRED HINTS ${PROJECT_SOURCE_DIR}/lib/stb)
find_package(chicken3421 REQUIRED HINTS ${PROJECT_SOURCE_DIR}/lib/chicken3421)

set(COMMON_LIBS glad::glad glm::glm glfw stb chicken3421)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

include(copy_resources)
copy_resources(${CMAKE_CURRENT_LIST_DIR}/res ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/res)

add_executable(ass1)
target_include_directories(ass1 PUBLIC include)
target_sources(ass1 PRIVATE ${PROJECT_SOURCE_DIR}/src/main.cpp)
target_link_libraries(ass1 PUBLIC ${COMMON_LIBS})