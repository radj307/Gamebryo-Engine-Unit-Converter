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

# Unit Testing
include(CTest)

add_test(NAME ckconv-test-intra-metric COMMAND
	"ckconv"
	"-nq"
	"pm"	"1000"	"nm"
	"nm"	"1000"	"um"
	"um"	"1000"	"mm"
	"mm"	"10"	"cm"
	"cm"	"10"	"dm"
	"dm"	"10"	"m"
	"m"		"10"	"dam"
	"dam"	"10"	"hm"
	"hm"	"10"	"km"
	"km"	"1000"	"Mm"
	"Mm"	"1000"	"Gm"
	"Gm"	"1000"	"Tm"
	"Tm"	"1"		"m"
)
add_test(NAME ckconv-test-intra-creationkit COMMAND
	"ckconv"
	"-nq"
	"pu"	"1000"	"nu"
	"nu"	"1000"	"uu"
	"uu"	"1000"	"mu"
	"mu"	"10"	"cu"
	"cu"	"10"	"du"
	"du"	"10"	"u"
	"u"		"10"	"dau"
	"dau"	"10"	"hu"
	"hu"	"10"	"ku"
	"ku"	"1000"	"Mu"
	"Mu"	"1000"	"Gu"
	"Gu"	"1000"	"Tu"
	"Tu"	"1"		"u"
)
add_test(NAME ckconv-test-intra-imperial COMMAND
	"ckconv"
	"-nq"
	"pu"	"1000"	"nu"
	"nu"	"1000"	"uu"
	"uu"	"1000"	"mu"
	"mu"	"10"	"cu"
	"cu"	"10"	"du"
	"du"	"10"	"u"
	"u"		"10"	"dau"
	"dau"	"10"	"hu"
	"hu"	"10"	"ku"
	"ku"	"1000"	"Mu"
	"Mu"	"1000"	"Gu"
	"Gu"	"1000"	"Tu"
	"Tu"	"1"		"u"
)

