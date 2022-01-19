# GamebryoUnitConv/UnitConv
cmake_minimum_required(VERSION 3.15)

if (MSVC)
	set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

# Create project
project("ckconv" VERSION "${GamebryoUnitConv_VERSION}" LANGUAGES CXX)

# Get the list of headers
file(GLOB HEADERS
	RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}"
	CONFIGURE_DEPENDS
	"*.h*"
)

# Include modules
include(ResourceMaker)

# Create version header
CREATE_VERSION_HEADER("ckconv" "${GamebryoUnitConv_VERSION_MAJOR}" "${GamebryoUnitConv_VERSION_MINOR}" "${GamebryoUnitConv_VERSION_PATCH}")
# Create versioninfo & icon .rc file
CREATE_VERSION_RESOURCE("${CMAKE_CURRENT_BINARY_DIR}/ckconv.rc" "${GamebryoUnitConv_VERSION_MAJOR}" "${GamebryoUnitConv_VERSION_MINOR}" "${GamebryoUnitConv_VERSION_PATCH}" "radj307" "Converts between metric, imperial, and creation kit length units." "ckconv" "GPLv3" "ckconv.exe" "Gamebryo/CreationKit Engine Unit Converter")
APPEND_ICON_RESOURCE("${CMAKE_CURRENT_BINARY_DIR}/ckconv.rc" "${CMAKE_CURRENT_SOURCE_DIR}/MAINICON.ico")

# Create executable
add_executable(ckconv "main.cpp" "${CMAKE_CURRENT_BINARY_DIR}/ckconv.rc")

# Set properties
set_property(TARGET ckconv PROPERTY CXX_STANDARD 20)
set_property(TARGET ckconv PROPERTY CXX_STANDARD_REQUIRED ON)
if (MSVC)
	target_compile_options(ckconv PUBLIC "/Zc:__cplusplus")
endif()

# Add headers
include(PrependEach)
PREPEND_EACH(HEADERS_ABS "${HEADERS}" "${CMAKE_CURRENT_SOURCE_DIR}")
target_sources(ckconv PUBLIC 
	"$<BUILD_INTERFACE:${HEADERS_ABS}>"
	"$<INSTALL_INTERFACE:${HEADERS}>"
)

# Link dependencies
target_link_libraries(ckconv PUBLIC TermAPI optlib filelib xlog)

# Create installation targets
include(PackageInstaller)
INSTALL_EXECUTABLE(ckconv "${CMAKE_INSTALL_PREFIX}/bin/")
