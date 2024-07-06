CC?=gcc
SDL2FLAGS:=$(shell pkg-config sdl2 --cflags --libs) -lSDL2_ttf
CFLAGS?:=-std=c2x -Wall -pedantic -Werror -Wshadow -Wstrict-aliasing -Wstrict-overflow
OUT_NAME=kbdtestsdl2
.PHONY: all msg clean fullclean

all: msg ${OUT_NAME}

msg:
	@echo '--- C2X ---'

${OUT_NAME}: ${OUT_NAME}.c
	${CC} ${CFLAGS} -O2 -o $@ $< ${SDL2FLAGS}

small: ${OUT_NAME}.c
	${CC} ${CFLAGS} -Os -o ${OUT_NAME} $< ${SDL2FLAGS}
	-strip ${OUT_NAME}
	-sstrip ${OUT_NAME}

debug: ${OUT_NAME}.c
	${CC} ${CFLAGS} -O1 -g -o ${OUT_NAME} $< ${SDL2FLAGS}

asm: ${OUT_NAME}.asm

${OUT_NAME}.asm: ${OUT_NAME}.c
	${CC} ${CFLAGS} -S -masm=intel -Og -o ${OUT_NAME} $< ${SDL2FLAGS}

run: msg ${OUT_NAME}
	./${OUT_NAME}

clean:
	rm -f ${OUT_NAME} *.o ${OUT_NAME}.asm

fullclean: clean

