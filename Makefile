############################################################
# 						 CMAKE CMDS
############################################################

# build the cmake project
# DEFAULT CMD
cmake:
	mkdir build || \
		cd ./build && \
		cmake .. && \
		cmake --build .

# clean build dir
clean:
	del /q /S build

# run the cmake executable
make run:
	.\build\Debug\WiimotePS2PuppetAdapter.exe

############################################################
# 						ARDUINO CMDS
############################################################

# flash the given project to a board
emb_flash:
	cd $1 && \
		pio run --t upload --environment uno

# compile NOT upload the given project
emb_compile:
	cd $1 && \
		pio run -e uno
