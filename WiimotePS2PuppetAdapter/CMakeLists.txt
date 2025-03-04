# source files = $SRC_FILES
add_subdirectory(./src)
# headers
include_directories(./include)
# build executable
add_executable(WiimotePS2PuppetAdapter ${SRC_FILES})

####################################################
#                   LIBRARIES
####################################################
add_subdirectory(./lib/wiiuse)
add_subdirectory(./lib/simpleble/simpleble)
target_include_directories(
    WiimotePS2PuppetAdapter PRIVATE
    ./lib/wiiuse/src
    ./lib/simpleble/simpleble/include/simpleble_c
    ./include
)
target_link_libraries(WiimotePS2PuppetAdapter wiiuse)
target_link_libraries(WiimotePS2PuppetAdapter simpleble::simpleble-c)
