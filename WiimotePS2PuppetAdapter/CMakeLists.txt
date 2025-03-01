# source files = $SRC_FILES
add_subdirectory(./src)
# headers
include_directories(./include)
# build executable
add_executable(WiimotePS2PuppetAdapter ${SRC_FILES})

####################################################
#                   LIBRARIES
####################################################
include_directories(
    ${CMAKE_SOURCE_DIR}/wiiuse/src
    ${CMAKE_SOURCE_DIR}/simpleble/simpleble/include/simpleble_c
)
target_link_libraries(WiimotePS2PuppetAdapter wiiuse)
target_link_libraries(WiimotePS2PuppetAdapter simpleble::simpleble)
