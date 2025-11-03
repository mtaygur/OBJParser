# CMakeList.txt : CMake project for OBJParser, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.15)


project ("OBJParser")

# Project-wide C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME ON)

find_package(Boost 1.63 REQUIRED COMPONENTS filesystem)
include_directories(${Boost_INCLUDE_DIRS})

# Add source to this project's executable.
add_library(OBJParser_lib STATIC
    "src/OBJParser.cpp"
    "src/OBJParser.h"
)

target_include_directories(OBJParser_lib
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(OBJParser_lib PUBLIC Boost::filesystem)

# optional: small app that uses the library
add_executable(OBJParser_exe src/main.cpp)
target_link_libraries(OBJParser_exe PRIVATE OBJParser_lib)

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
		target_link_libraries(OBJParser_exe ${PROFILING_LIB} )
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
		target_link_libraries(OBJParser_exe ${PROFILING_LIB} )
	endif()

endif()


include(CTest)
if (BUILD_TESTING)
    # Use vcpkg-installed googletest (expects vcpkg toolchain to be used)
    find_package(GTest CONFIG REQUIRED)

    add_executable(OBJParser_tests
        test/OBJParserTests.cpp
    )

    target_link_libraries(OBJParser_tests
        PRIVATE
            OBJParser_lib
            GTest::gtest_main
    )

    target_include_directories(OBJParser_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
    target_compile_features(OBJParser_tests PRIVATE cxx_std_20)

    add_test(NAME OBJParserTests COMMAND OBJParser_tests)
endif()
