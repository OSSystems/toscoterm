all: main.c

main.c:
	 gcc -o toscoterm $(shell pkg-config --libs --cflags gtk+-2.0) $(shell pkg-config --libs --cflags vte) main.c

clean:
	rm -f toscoterm

install: main.c
	install -o root -g root -m 644 toscoterm $(DESTDIR)/$(PREFIX)/bin/

