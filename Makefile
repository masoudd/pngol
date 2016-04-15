CFLAGS := -std=c99 -Wall -lpng

pngol: life.c life.h config.c config.h
	gcc $(CFLAGS) $(CLIBS) life.c config.c -o $@

clean:
	rm pngol

.PHONEY: clean
