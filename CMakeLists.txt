# GamebryoUnitConv
cmake_minimum_required (VERSION 3.15)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/307lib/307modules")

include(AutoVersion)
GET_VERSION("${CMAKE_CURRENT_SOURCE_DIR}" GamebryoUnitConv_VERSION_MAJOR GamebryoUnitConv_VERSION_MINOR GamebryoUnitConv_VERSION_PATCH)
MAKE_VERSION(GamebryoUnitConv_VERSION "${GamebryoUnitConv_VERSION_MAJOR}" "${GamebryoUnitConv_VERSION_MINOR}" "${GamebryoUnitConv_VERSION_PATCH}")

# Create project
project("GamebryoUnitConv" VERSION ${GamebryoUnitConv_VERSION} LANGUAGES CXX)

add_subdirectory ("307lib")

add_subdirectory ("ckconv")
