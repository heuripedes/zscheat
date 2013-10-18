
BIN=zscheat
SOURCE=zscheat.c
OBJECTS=$(SOURCE:.c=.o)
CFLAGS=-Wall -g -O2

PREFIX=/usr/local

all: $(BIN)

$(BIN): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $<

.c.o:
	$(CC) $(CFLAGS) -c -o $@ $<

install: all
	mkdir -p $(PREFIX)/bin
	install -m 755 $(BIN) $(PREFIX)/bin

uninstall:
	-rm $(PREFIX)/bin/$(BIN)

