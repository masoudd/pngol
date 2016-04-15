CFLAGS := -std=c99 -Wall -lpng

Life: life.c life.h config.c config.h
	gcc $(CFLAGS) $(CLIBS) life.c config.c -o $@

clean:
	rm Life

.PHONEY: clean
