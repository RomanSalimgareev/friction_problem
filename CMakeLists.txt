cmake_minimum_required(VERSION 3.19)
project(friction-app LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(libs/MFE_lib)
find_package(matplotlib_cpp REQUIRED)

set(sources apps/friction_main.cpp)

add_executable(${PROJECT_NAME})

target_sources(${PROJECT_NAME} PRIVATE ${sources})

set_target_properties(${PROJECT_NAME} PROPERTIES
	 RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

target_link_libraries(${PROJECT_NAME}
	PUBLIC 
		MFE_lib::MFE_lib
		matplotlib_cpp::matplotlib_cpp
)