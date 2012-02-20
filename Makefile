CC     ?= gcc
CFLAGS  ?= $(shell pkg-config --cflags vte) -Os -Wall
LDFLAGS ?= $(shell pkg-config --libs vte)
TARGET  = toscoterm
SOURCES = main.c
OBJECTS = $(SOURCES:.c=.o)
PREFIX ?= /usr

all: $(SOURCES) $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

clean:
	rm -f *.o
	rm -f toscoterm

install: all
	install -d $(DESTDIR)$(PREFIX)/bin/
	install -m 755 toscoterm $(DESTDIR)$(PREFIX)/bin/
	install -d $(DESTDIR)$(PREFIX)/share/toscoterm/
	install -m 644 icon.png $(DESTDIR)$(PREFIX)/share/toscoterm/
