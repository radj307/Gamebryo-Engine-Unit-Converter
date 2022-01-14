# GamebryoUnitConv/UnitConv
cmake_minimum_required(VERSION 3.15)

if (MSVC)
	set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

# Create project
project("ckconv" VERSION "${GamebryoUnitConv_VERSION}" LANGUAGES CXX)

file(GLOB HEADERS
	RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}"
	CONFIGURE_DEPENDS
	"*.h*"
)

# Create executable
add_executable(ckconv "main.cpp")

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
target_link_libraries(ckconv PUBLIC TermAPI optlib filelib)

# Create installation targets
include(PackageInstaller)
INSTALL_EXECUTABLE(ckconv "${CMAKE_INSTALL_PREFIX}/bin/")
