
get_filename_component(RESOURCES_PATH "${CMAKE_SOURCE_DIR}/Resources" ABSOLUTE)
add_definitions(-DRES_DIR="${RESOURCES_PATH}")