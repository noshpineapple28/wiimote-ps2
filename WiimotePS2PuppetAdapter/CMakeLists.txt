# source files = $SRC_FILES
add_subdirectory(./src)
add_subdirectory(./lib/wiiuse/src)
# headers
include_directories(./include)
include_directories(./lib/wiiuse/src)
# build executable
add_executable(WiimotePS2PuppetAdapter ${SRC_FILES})

####################################################
#                   LIBRARIES
####################################################
add_subdirectory(./lib/simpleble/simpleble)
target_include_directories(
    WiimotePS2PuppetAdapter PRIVATE
    ./lib/simpleble/simpleble/include/simpleble_c
)
# target_link_libraries(WiimotePS2PuppetAdapter wiiuse)
target_link_libraries(WiimotePS2PuppetAdapter simpleble::simpleble-c)
