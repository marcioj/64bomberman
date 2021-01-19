GAME = game
ROM = $(GAME).n64
BUILD_DIR = build
SRC_DIRS = src lib
N64_LIB = /usr/lib/n64
N64_INC = /usr/include/n64
NUSYSINCDIR = $(N64_INC)/nusys
NUSYSLIBDIR = $(N64_LIB)/nusys
NUSTDINCDIR = $(N64_INC)/nustd

CC = mips64-elf-gcc
LD = mips64-elf-ld

CDEFS = -DALIGNED8='__attribute((aligned(8)))' -D_MIPS_SZLONG=32 -D_MIPS_SZINT=32 -D_LANGUAGE_C -D_ULTRA64 -D__EXTENSIONS__ -DNU_DEBUG -DF3DEX_GBI_2
COPTS = -ffreestanding -fno-PIC -mabi=32 -mno-shared -mno-abicalls -march=vr4300 -mtune=vr4300 -mfix4300 -G 0 -g -MMD -MP \
	-Werror=return-type -Werror=incompatible-pointer-types
CINC =  -Isrc -Ilib -I$(NUSYSINCDIR) -I$(NUSTDINCDIR) -I$(N64_INC)/PR -I$(N64_INC)
CFLAGS = $(CDEFS) $(COPTS) $(CINC)
NUAUDIOLIB = -ln_audio_d -lnualstl_n_d -ln_mus_d -ln_audio_sc
LDFLAGS = -L$(N64_LIB) -L$(NUSYSLIBDIR) -L$(N64_LIBGCCDIR) $(NUAUDIOLIB) -lnusys_d -lultra_d -lgcc -lnustd_d

CODEFILES := $(shell find $(SRC_DIRS) -name '*.c')
CODEOBJECTS = $(CODEFILES:%.c=$(BUILD_DIR)/%.o) $(NUSYSLIBDIR)/nusys.o
CODEDEPS := $(CODEFILES:%.c=$(BUILD_DIR)/%.d)
CODESEGMENT = $(BUILD_DIR)/codesegment.o

rom: $(ROM)

console: CDEFS += -DCONSOLE
console: rom

default: rom

$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(CODESEGMENT):	$(CODEOBJECTS) Makefile
	$(LD) -o $(CODESEGMENT) -r $(CODEOBJECTS) $(LDFLAGS)

$(ROM):	$(CODESEGMENT)
	spicy spec -I$(NUSYSINCDIR) -r $(ROM) -s 9 -e $(GAME).out
	makemask $(ROM)

clean:
	@rm -rf build *.out *.d *.n64 assets/sounds/*.o

.PHONY: default clean console rom

-include $(CODEDEPS)
