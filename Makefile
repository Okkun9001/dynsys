CC = gcc
CFLAGS = -Wall -Wextra -Wno-unused -Wpedantic

TARGET= dynsys
SOURCES = src/main.c

.PHONY: all run clean

all: $(TARGET)

$(TARGET) : $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES) -o $(TARGET)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f dynsys
