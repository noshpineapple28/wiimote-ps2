# source files = $SRC_FILES
add_subdirectory(./wiiuse_src)
# headers
include_directories(./wiiuse_src)

# source files
set(
    SRC_FILES
    ${CMAKE_CURRENT_LIST_DIR}/main.c
    ${CMAKE_CURRENT_LIST_DIR}/ble_handler.c
    ${CMAKE_CURRENT_LIST_DIR}/wiimote_handler.c
    ${SOURCES}
    PARENT_SCOPE
)
