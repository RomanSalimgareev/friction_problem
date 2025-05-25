cmake_minimum_required(VERSION 3.19 FATAL_ERROR)

project(MFE_lib 
	VERSION 1.0.0
	DESCRIPTION "MFE functions and solvers for the friction problem"
	LANGUAGES CXX
)

file(TO_CMAKE_PATH 
	"${PROJECT_BINARY_DIR}/CMakeLists.txt" 
	LOC_PATH
)

if(EXISTS "${LOC_PATH}")
	message(FATAL_ERROR "You can't build a project\
	outside of the build directory")
endif()

include(cmake/utils.cmake)
include(GNUInstallDirs)


# Settings and options

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

string(COMPARE EQUAL "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}" 
	is_top_level
)

option(MFE_LIB_INSTALL "Generate target for installing MFE_lib" ${is_top_level})
set_if_undefined(MFE_LIB_INSTALL_CMAKEDIR 
	"${CMAKE_INSTALL_LIBDIR}/cmake/MFE_lib" CACHE STRING
    "Install path for MFE_lib package-related CMake files"
)

if(DEFINED MFE_LIB_SHARED_LIBS)
    set(BUILD_SHARED_LIBS ${MFE_LIB_SHARED_LIBS})
endif()

if (NOT DEFINED CMAKE_BUILD_TYPE AND NOT DEFINED CMAKE_CONFIGURATION_TYPES)
	set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
	set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS 
	"Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

set_if_undefined(CMAKE_CXX_VISIBILITY_PRESET hidden)
set_if_undefined(CMAKE_VISIBILITY_INLINES_HIDDEN ON)

# Target MFE_lib and include sources and headers files
# Uses the BUILD_SHARED_LIBS variable by default to build statically or dynamically.
add_library(MFE_lib "")
add_library(MFE_lib::MFE_lib ALIAS MFE_lib)

# Headers and sources files

include(GenerateExportHeader)
set(export_file_name "export_shared.h")

if(NOT BUILD_SHARED_LIBS)
	set(export_file_name "export_static.h")
endif()

generate_export_header(MFE_lib EXPORT_FILE_NAME 
	${CMAKE_CURRENT_SOURCE_DIR}/include/MFE_lib/${export_file_name}
)

set(HEADERS_PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

set(HEADERS_PUBLIC 
	${CMAKE_CURRENT_SOURCE_DIR}/include
)

file(GLOB_RECURSE SOURCES_PRIVATE CONFIGURE_DEPENDS
	src/math_functions.cpp
	src/class_matrix.cpp
	src/write_functions.cpp
	src/get_disps_func.cpp
	src/functions_for_MFE/*.cpp
	src/error_handling/*.cpp
	src/operator_overloading/*.cpp
	src/inputFunctions/*.cpp
)

target_sources(MFE_lib
	PRIVATE
		${SOURCES_PRIVATE}
)

target_include_directories(MFE_lib
	PUBLIC	
		$<BUILD_INTERFACE:${HEADERS_PRIVATE}>
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
	PUBLIC
		$<BUILD_INTERFACE:${HEADERS_PUBLIC}>
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_compile_definitions(MFE_lib
	PUBLIC
		"$<$<NOT:$<BOOL:${BUILD_SHARED_LIBS}>>:MFE_LIB_STATIC_DEFINE>"
)

set(HAS_DEBUG FALSE)
foreach(config ${CMAKE_CONFIGURATION_TYPES})
	if(config STREQUAL "Debug")
		set(HAS_DEBUG TRUE)
		break()
	endif()
endforeach()

if(NOT (CMAKE_BUILD_TYPE STREQUAL "Debug") OR NOT HAS_DEBUG)
target_compile_definitions(MFE_lib
	PRIVATE 
		NDEBUG
)
endif()

set_target_properties(MFE_lib PROPERTIES
	SOVERSION ${PROJECT_VERSION_MAJOR}
	VERSION ${PROJECT_VERSION}
)

include(CMakePackageConfigHelpers)

if(MFE_LIB_INSTALL AND NOT CMAKE_SKIP_INSTALL_RULES)
	configure_package_config_file(
		cmake/MFE_libConfig.cmake.in
		MFE_libConfig.cmake
		INSTALL_DESTINATION "${MFE_LIB_INSTALL_CMAKEDIR}"
	)
	
	write_basic_package_version_file(MFE_libConfig-version.cmake
		VERSION ${PROJECT_VERSION}
		COMPATIBILITY SameMajorVersion
	)

	install(TARGETS MFE_lib 
		EXPORT MFE_lib_export
		RUNTIME COMPONENT MFE_lib
		LIBRARY 
			COMPONENT MFE_lib 
			NAMELINK_COMPONENT MFE_lib_dev
		ARCHIVE COMPONENT MFE_lib_dev 
		INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
	)

# If you enter TYPE INCLUDE, cmake will determine the installation
# location for ${CMAKE_INSTALL_INCLUDEDIR}
	install(DIRECTORY include/
		TYPE INCLUDE 
		COMPONENT MFE_lib_dev
		#DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
	)
	
	install(FILES share/README.md
		DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}
	)
	
	install(DIRECTORY share/images
		DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}
	)
	
	set(targets_file "MFE_lib-shared-targets.cmake")
	
	if(NOT BUILD_SHARED_LIBS)
		set(targets_file "MFE_lib-static-targets.cmake")
	endif()
	
	install(EXPORT MFE_lib_export
		COMPONENT MFE_lib_dev
		FILE "${targets_file}"
		DESTINATION "${MFE_LIB_INSTALL_CMAKEDIR}"
		NAMESPACE MFE_lib::
	)
	
	install(FILES
		"${CMAKE_CURRENT_BINARY_DIR}/MFE_libConfig.cmake"
		"${CMAKE_CURRENT_BINARY_DIR}/MFE_libConfig-version.cmake"
		COMPONENT MFE_lib_dev
		DESTINATION "${MFE_LIB_INSTALL_CMAKEDIR}"
	)
	
	if(MSVC)
		set(pdb_file "")
		set(pdb_file_destination "")
		
		if(BUILD_SHARED_LIBS)
			set(pdb_file $<TARGET_PDB_FILE:MFE_lib>)
			set(pdb_file_destination ${CMAKE_INSTALL_BINDIR})
		else()
			set(pdb_file 
			"$<TARGET_FILE_DIR:MFE_lib>/"
			"$<TARGET_FILE_PREFIX:MFE_lib>/"
			"$<TARGET_FILE_BASE_NAME:MFE_lib>.pdb")
			set(pdb_file_destination ${CMAKE_INSTALL_LIBDIR})
		endif()
		
		install(FILES "${pdb_file}"
			COMPONENT MFE_lib_dev
			CONFIGURATIONS Debug RelWithDebInfo
			DESTINATION "${pdb_file_destination}"
			OPTIONAL
		)
	endif()
	
endif()

