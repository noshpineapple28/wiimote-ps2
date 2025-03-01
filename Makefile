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
run:
	.\build\WiimotePS2PuppetAdapter\Debug\WiimotePS2PuppetAdapter.exe

############################################################
# 						ARDUINO CMDS
############################################################

# flash the given project to a board
emb_flash:
	cd $1 && \
		make emb_flash

# compile NOT upload the given project
emb_compile:
	cd $1 && \
		make emb_compile
