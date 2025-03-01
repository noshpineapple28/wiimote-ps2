# general metadata
cmake_minimum_required(VERSION 3.10)
project(WiimotePS2PuppetAdapter)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)

# link the library source files
add_subdirectory(./WiimotePS2PuppetAdapter)
add_subdirectory(./wiiuse)
add_subdirectory(./simpleble/simpleble)
