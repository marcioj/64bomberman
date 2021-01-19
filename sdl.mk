GAME = game
BUILD_DIR = build
SRC_DIRS = src lib

CDEFS = -DALIGNED8='__attribute((aligned(8)))'
COPTS = -g -MMD -MP -Werror=return-type -Werror=incompatible-pointer-types
CINC =  -Isrc -Ilib
CFLAGS = $(CDEFS) $(COPTS) $(CINC)
LDFLAGS = -lc -lm -lSDL2 -lSDL2_image -lSDL2_ttf

# CODEFILES := $(shell find $(SRC_DIRS) -name '*.c')
CODEFILES := main.c src/clock.c src/world.c src/player.c src/camera.c src/game_object.c src/level.c \
             src/animation.c src/bomb.c src/block.c src/math_utils.c lib/vec.c src/aabb.c \
             src/controller.c src/enemy.c src/texture.c src/textures.c src/grid.c src/collision.c
CODEOBJECTS = $(CODEFILES:%.c=$(BUILD_DIR)/%.o)
CODEDEPS := $(CODEFILES:%.c=$(BUILD_DIR)/%.d)

default: $(GAME)

$(GAME): Makefile $(CODEFILES)
	$(CC) $(CFLAGS) $(CODEFILES) $(LDFLAGS) -o $(GAME)

clean:
	@rm -rf $(GAME) *.d

.PHONY: default clean console rom

-include $(CODEDEPS)
