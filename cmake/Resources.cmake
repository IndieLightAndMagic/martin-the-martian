
get_filename_component(RESOURCES_PATH "${CMAKE_SOURCE_DIR}/Resources" ABSOLUTE)
add_definitions(-DRES_DIR="${RESOURCES_PATH}/")

get_filename_component(GTECH_RESOURCES_PATH "${CMAKE_SOURCE_DIR}/src/Tech/Resources" ABSOLUTE)
add_definitions(-DGTECH_RES_DIR="${GTECH_RESOURCES_PATH}/")


message(STATUS "Resources directory is: ${RESOURCES_PATH}")
message(STATUS "Resources directory is: ${GTECH_RESOURCES_PATH}")
