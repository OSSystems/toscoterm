CC		= gcc -Wall
CFLAGS	= $(shell pkg-config --cflags gtk+-2.0) $(shell pkg-config --cflags vte)
LDFLAGS	= $(shell pkg-config --libs gtk+-2.0) $(shell pkg-config --libs vte)
TARGET	= toscoterm
SOURCES	= main.c
OBJECTS = $(SOURCES:.c=.o)

all: $(SOURCES) $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

clean:
	rm -f *.o
	rm -f toscoterm

install: main.c
	install -o root -g root -m 644 toscoterm $(DESTDIR)/$(PREFIX)/bin/

