cmake_minimum_required(VERSION 3.23 FATAL_ERROR)

file(TO_CMAKE_PATH 
	"${PROJECT_BINARY_DIR}/CMakeLists.txt" 
	LOC_PATH
)
if (EXISTS "${LOC_PATH}")
	message(FATAL_ERROR "You can't build a project\
	outside of the build directory")
endif()

set(MAIN_TARGET diploma)

project(Diploma 
	DESCRIPTION "The problem of body movement\
	with friction and functions MFE"
	LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(NAME_EXE diploma)

add_executable(${NAME_EXE} 
	${CMAKE_SOURCE_DIR}/apps/diploma_main.cpp
)

if(NOT DEFINED CMAKE_BUILD_TYPE AND 
	NOT DEFINED CMAKE_CONFIGURATION_TYPES)
		set(CMAKE_BUILD_TYPE Release 
			CACHE STRING "Build type" FORCE)
		set_property(CACHE CMAKE_BUILD_TYPE PROPERTY
			STRINGS "Release" "Debug" "MinSizeRel" "RelWithDebInfo")
endif()	


set_target_properties(${NAME_EXE} PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

add_subdirectory(src)

target_link_libraries(${NAME_EXE} MFE) 