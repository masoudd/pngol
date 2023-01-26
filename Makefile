CFLAGS := -std=c99 -Wall -lpng

pngol: life.c life.h config.c config.h
	gcc $(CFLAGS) $(CLIBS) life.c config.c -o $@

debug:
	gcc $(CFLAGS) $(CLIBS) life.c config.c -g -o $@

clean:
	rm pngol debug

.PHONEY: clean
