GAME = game
BUILD_DIR = build
SRC_DIRS = src lib

CDEFS = -DALIGNED8='__attribute((aligned(8)))'
COPTS = -g -MMD -MP -Wfatal-errors -Werror=return-type -Werror=incompatible-pointer-types
CINC =  -Isrc -Ilib
CFLAGS = $(CDEFS) $(COPTS) $(CINC)
LDFLAGS = -lc -lm -lSDL2 -lSDL2_image -lSDL2_ttf

# CODEFILES := $(shell find $(SRC_DIRS) -name '*.c')
CODEFILES := main.c src/clock.c src/controller.c src/texture.c src/textures.c  \
             src/sprite_sheet.c src/tile.c src/animation.c lib/vec.c src/aabb.c  \
             src/game_object.c src/level.c  src/camera.c src/world.c src/player.c \
             src/grid.c src/collision.c src/math_utils.c src/block.c src/bomb.c src/enemy.c
CODEOBJECTS = $(CODEFILES:%.c=$(BUILD_DIR)/%.o)
CODEDEPS := $(CODEFILES:%.c=$(BUILD_DIR)/%.d)

default: $(GAME)

$(GAME): Makefile $(CODEFILES)
	$(CC) $(CFLAGS) $(CODEFILES) $(LDFLAGS) -o $(GAME)

clean:
	@rm -rf $(GAME) *.d

.PHONY: default clean console rom

-include $(CODEDEPS)
