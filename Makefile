# force to shit born to write a makefile.
# i have no clue how this works still but whatever.r.

BUILD_SRC 		= $(wildcard src/*.cpp)
BUILD_FILES  	= $(patsubst src/%.cpp, build/%, $(BUILD_SRC))

CC 			= clang
CFLAGS  	= -pthread -g -DSOKOL_NO_ENTRY \
				-Wno-c99-designator -Wno-c++20-designator -Wno-unused-value
LDFLAGS 	= -lGL -ldl -lm -lX11 -lasound -lXi -lXcursor -lstdc++
INCLUDES 	= -Ilibs/sokol -Ilibs/image -Ilibs/sokol/util -Ilibs

# BASS
CFLAGS += -Wl,-rpath=libs/bass/DLLs/,-Llibs/bass/DLLs/
LDFLAGS += -lbass -lbass_fx -lbassmix
INCLUDES += -Ilibs/bass

# CFLAGS += -O2

.PHONY: all
all: build/ $(BUILD_FILES)
	@echo $(BUILD_FILES)

.PHONY: clear
clear: 
	rm -rf build/

.PHONY: run
run: clear all 
	./build/main

build/:
	mkdir -p build/

build/%: src/%.cpp
	$(CC) -o $@ $< libs/sokol/include.c libs/image/include.c libs/bass/include.c $(CFLAGS) $(LDFLAGS) $(INCLUDES) 