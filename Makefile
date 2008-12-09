CC      ?= gcc
CFLAGS	= $(shell pkg-config --cflags gtk+-2.0) $(shell pkg-config --cflags vte) -Os -Wall
LDFLAGS	= $(shell pkg-config --libs gtk+-2.0) $(shell pkg-config --libs vte)
TARGET	= toscoterm
SOURCES	= main.c
OBJECTS = $(SOURCES:.c=.o)
PREFIX ?= /usr

all: $(SOURCES) $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

clean:
	rm -f *.o
	rm -f toscoterm

install: main.c
	install -d $(DESTDIR)$(PREFIX)/bin/
	install -m 755 toscoterm $(DESTDIR)$(PREFIX)/bin/
