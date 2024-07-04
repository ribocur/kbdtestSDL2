CC?=gcc
SDL2FLAGS:=$(shell pkg-config sdl2 --cflags --libs)
CFLAGS?:=-std=c2x -Wall -pedantic -Werror -Wshadow -Wstrict-aliasing -Wstrict-overflow

.PHONY: all msg clean fullclean

all: msg main

msg:
	@echo '--- C2X ---'

main: main.c
	${CC} ${CFLAGS} -O2 -o $@ $< ${SDL2FLAGS}

small: main.c
	${CC} ${CFLAGS} -Os -o main $< ${SDL2FLAGS}
	-strip main
	-sstrip main

debug: main.c
	${CC} ${CFLAGS} -O1 -g -o main $< ${SDL2FLAGS}

asm: main.asm

main.asm: main.c
	${CC} ${CFLAGS} -S -masm=intel -Og -o main.asm $< ${SDL2FLAGS}

run: msg main
	./main

clean:
	rm -f main *.o main.asm

fullclean: clean

