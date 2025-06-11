# CMakeList.txt : CMake project for OBJParser, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.15)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  #cmake_policy(SET CMP0141 NEW)
  #set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("OBJParser")

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME ON)

find_package(Boost 1.63 REQUIRED COMPONENTS filesystem)
include_directories(${Boost_INCLUDE_DIRS})

# Add source to this project's executable.
add_executable (OBJParser "OBJParser.cpp" "OBJParser.h")
target_link_libraries(OBJParser Boost::filesystem)

set_property(TARGET OBJParser PROPERTY CXX_STANDARD 20)
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	add_compile_options(-Wall -Wextra -Wpedantic)
elseif(MSVC)
	add_definitions(/W4)
endif()


if(CMAKE_BUILD_TYPE STREQUAL "Debug")

	if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
		add_compile_options(-g)
		add_definitions(-DDEBUG)
	elseif(MSVC)
		add_compile_options(/Zi)
		add_definitions(/DEBUG)
	endif()
	if(ENABLE_PROFILER STREQUAL "ON" AND MSVC)
		add_definitions(/GH /Gh)
		find_library(PROFILING_LIB micro-profiler_x64)
		target_link_libraries(OBJParser ${PROFILING_LIB} )
	endif()

elseif(CMAKE_BUILD_TYPE STREQUAL "Release" OR CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")

	if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
		add_definitions(-O2)
	elseif(MSVC)
		add_definitions(/O2)
	endif()
	if(ENABLE_PROFILER STREQUAL "ON" AND MSVC)
		add_definitions(/GH /Gh)
		find_library(PROFILING_LIB micro-profiler_x64)
		target_link_libraries(OBJParser ${PROFILING_LIB} )
	endif()

endif()

# TODO: Add tests and install targets if needed.
