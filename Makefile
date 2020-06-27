.PHONY: clean

all: life glife

life: life.c lifelib.c
	@gcc -o life life.c lifelib.c

glife: glife.c lifelib.c
	@gcc -o glife glife.c lifelib.c `sdl2-config --cflags --libs`

clean:
	@rm -f life glife
