CFLAGS := -std=c99 -Wall -lpng

pngol: life.c life.h config.c config.h
	gcc $(CFLAGS) $(CLIBS) life.c config.c -o $@

debug:
	gcc $(CFLAGS) $(CLIBS) life.c config.c -g -o $@

clean:
	rm -f pngol debug

.PHONEY: clean
