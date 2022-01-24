# GamebryoUnitConv
cmake_minimum_required (VERSION 3.15)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/307lib/307modules")

if (DEFINED $ENV{GamebryoUnitConv_VERSION})
	message(STATUS "Using version from environment")
	set(GamebryoUnitConv_VERSION $ENV{GamebryoUnitConv_VERSION} CACHE STRING "" FORCE)
	AV_PARSE_TAG("${GamebryoUnitConv_VERSION}" GamebryoUnitConv_VERSION_MAJOR GamebryoUnitConv_VERSION_MINOR GamebryoUnitConv_VERSION_PATCH)
else()
	message(STATUS "Using AutoVersion")
	include(AutoVersion2)
	GET_VERSION_2("GamebryoUnitConv" "${CMAKE_CURRENT_SOURCE_DIR}" MAJOR MINOR PATCH EXTRA)
endif()

# Create project
project("GamebryoUnitConv" VERSION ${GamebryoUnitConv_VERSION} LANGUAGES CXX)

add_subdirectory ("307lib")

add_subdirectory ("ckconv")
