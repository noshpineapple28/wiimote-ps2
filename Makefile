############################################################
# 						 CMAKE CMDS
############################################################

# build the cmake project
# DEFAULT CMD
MODE=Release

cmake:
	mkdir build || \
		cd ./build && \
		cmake .. && \
		cmake --build . --config $(MODE)

# clean build dir
clean:
	del /q /S build

# run the cmake executable
run:
	.\build\WiimotePS2PuppetAdapter\$(MODE)\WiimotePS2PuppetAdapter.exe

############################################################
# 						ARDUINO CMDS
############################################################

# flash the given project to a board
emb_flash:
	cd $(DIR) && \
		make emb_flash

# compile NOT upload the given project
emb_compile:
	cd $(DIR) && \
		make emb_compile
