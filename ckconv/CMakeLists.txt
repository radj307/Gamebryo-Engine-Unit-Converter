# GamebryoUnitConv/UnitConv
cmake_minimum_required(VERSION 3.15)

file(GLOB SRCS
	RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}"
	CONFIGURE_DEPENDS
	"*.c*"
)
file(GLOB HEADERS
	RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}"
	CONFIGURE_DEPENDS
	"*.h*"
)

include(VersionTag)
MAKE_VERSION_HEADER("${CMAKE_CURRENT_SOURCE_DIR}/version.h" "ckconv" "${CKCONV_VERSION}")

if (WIN32)
	include(ResourceMaker)
	MAKE_STRINGRC_VERSIONINFO(ckconv_versioninfo "${CKCONV_VERSION}" "GPLv3" "radj307" "ckconv"  "CreationKit Unit Converter Utility, supports metric, imperial, gamebryo/creation-kit \"units\", and additionally supports metric prefixes for units, such as the \"kilounit\".")
	MAKE_STRINGRC_ICON(ckconv_icon "${CMAKE_CURRENT_SOURCE_DIR}/resources/ckconv.ico")
	MAKE_RESOURCE("${CMAKE_CURRENT_SOURCE_DIR}/resources/ckconv.rc" "${ckconv_versioninfo}" "${ckconv_icon}")
endif()

# Create executable
add_executable(ckconv "${SRCS}" "${CMAKE_CURRENT_SOURCE_DIR}/resources/ckconv.rc")

# Set properties
set_property(TARGET ckconv PROPERTY CXX_STANDARD 20)
set_property(TARGET ckconv PROPERTY CXX_STANDARD_REQUIRED ON)
set_property(TARGET ckconv PROPERTY POSITION_INDEPENDENT_CODE ON)
if (MSVC)
	target_compile_options(ckconv PUBLIC "/Zc:__cplusplus" "/Zc:preprocessor")
endif()

# Add headers
target_sources(ckconv PUBLIC "${HEADERS}")

# Link dependencies
target_link_libraries(ckconv PUBLIC shared TermAPI optlib filelib)

# Create installation targets
include(PackageInstaller)
INSTALL_EXECUTABLE(ckconv "${CMAKE_INSTALL_PREFIX}/bin")
