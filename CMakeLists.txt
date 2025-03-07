# CMakeList.txt : CMake project for OBJParser, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("OBJParser")

#set(Boost_USE_STATIC_LIBS ON)
#set(Boost_USE_MULTITHREADED ON)
#set(Boost_USE_STATIC_RUNTIME OFF)

find_package(Boost 1.63 REQUIRED COMPONENTS filesystem)
include_directories(${Boost_INCLUDE_DIRS})

# Add source to this project's executable.
add_executable (OBJParser "OBJParser.cpp" "OBJParser.h")
target_link_libraries(OBJParser Boost::filesystem)

set_property(TARGET OBJParser PROPERTY CXX_STANDARD 20)

# TODO: Add tests and install targets if needed.
