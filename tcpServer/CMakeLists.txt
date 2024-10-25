cmake_minimum_required(VERSION 3.12.0)

project(tcpServer VERSION 0.0.3 LANGUAGES CXX C)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})
list(APPEND CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR})

SET(BUILD_PATH ${CMAKE_BINARY_DIR} CACHE STRING "Full build path")

if (NOT CMAKE_FROM_CONAN)
	if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
		message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
		file(DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/0.18.1/conan.cmake"
			"${CMAKE_BINARY_DIR}/conan.cmake"
			TLS_VERIFY ON)
	endif()

	include(${CMAKE_BINARY_DIR}/conan.cmake)

	set(CONAN_DIR ${CMAKE_SOURCE_DIR}/conan)

	if(MSVC)
		set(PROFILEBUILD ${CONAN_DIR}/msvc-profile CACHE STRING "")
	else()
		set(PROFILEBUILD ${CONAN_DIR}/gcc-13 CACHE STRING "")
	endif()
	if(NOT DEFINED PROFILEHOST)
		set(PROFILEHOST ${PROFILEBUILD} CACHE STRING "")
	endif()

	if(NOT DEFINED CONAN_BUILD_ARG)
		set(CONAN_BUILD_ARG "missing")
	endif()

	conan_cmake_autodetect(settings BUILD_TYPE ${CMAKE_BUILD_TYPE})
	set(ENV{CONAN_FROM_CMAKE} True) # Tell conanfile.py that he is being executed from the cmake
	set(settings "compiler=${CMAKE_CXX_COMPILER}") # compiler overriding to "Visual Studio, by smth. Fix this
	set(settings "build_type=${CMAKE_BUILD_TYPE}") # build_type overriding by smth. Fix this

	conan_cmake_install(
		PATH_OR_REFERENCE ${CONAN_DIR}
		BUILD "${CONAN_BUILD_ARG}"
		PROFILE_HOST ${PROFILEHOST}
		PROFILE_BUILD ${PROFILEBUILD}
		SETTINGS_HOST ${settings}
		SETTINGS_BUILD ${settings}
		OPTIONS "tcpServer/*:build_path=${BUILD_PATH}")
endif()

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Boost 1.76.0)


add_executable(tcpServer
    src/main.cpp
	include/SocketHandler.h
	src/SocketHandler.cpp)

target_include_directories(
    tcpServer
    PRIVATE
    ./src
    ./include)

target_link_libraries(tcpServer
    PRIVATE
    ${CMAKE_STATIC_BUILD}
	Boost::system
	Boost::filesystem
	Boost::thread)