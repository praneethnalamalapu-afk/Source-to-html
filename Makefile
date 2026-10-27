CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -Wpedantic -O2
TARGET = s2html
SOURCES = s2html_main.c s2html_event.c s2html_conv.c

all: $(TARGET)

$(TARGET): $(SOURCES) s2html_event.h s2html_conv.h
	$(CC) $(CFLAGS) $(SOURCES) -o $(TARGET)

clean:
	rm -f $(TARGET) $(TARGET).exe sample.c.html

.PHONY: all clean
