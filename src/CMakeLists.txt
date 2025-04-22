cmake_minimum_required(VERSION 3.23 FATAL_ERROR)

set (HEADERS_PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/error_handling/
	${CMAKE_CURRENT_SOURCE_DIR}/operator_overloading/
)

set (HEADERS_PUBLIC
	${CMAKE_SOURCE_DIR}/include/MFE_lib
	${CMAKE_SOURCE_DIR}/include/MFE_lib/functions_for_MFE/
	${CMAKE_SOURCE_DIR}/include/MFE_lib/types/
)

file (GLOB SOURCES_PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/math_functions.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/class_matrix.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/write_functions.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/functions_for_MFE/*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/error_handling/*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/operator_overloading/*.cpp
)

add_library(MFE STATIC "")

set_target_properties(MFE PROPERTIES
	ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib
)

target_sources(MFE
	PRIVATE
		${SOURCES_PRIVATE}
)

target_include_directories(MFE
	PRIVATE	
		${HEADERS_PRIVATE}
	PUBLIC
		${HEADERS_PUBLIC}
)

target_compile_definitions(MFE
	PRIVATE 
	NDEBUG
)

