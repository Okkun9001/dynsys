CC = gcc
CFLAGS = -Wall -Wextra -Wno-unused -Wpedantic \
         -I./src/core
SANITIZE_FLAGS = -fsanitize=address,undefined -g

SOURCES = src/main.c src/core/vmath.c
TARGET = dynsys
TEST_SOURCES = $(wildcard src/test/*.c)
TEST_TARGET = src/test/test_main


.PHONY: all run test sanitize clean

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES) -o $(TARGET)

test: src/test/test_main
	./src/test/test_main

src/test/test_main: $(TEST_SOURCES) src/core/vmath.c
	$(CC) $(CFLAGS) $^ -o $@ -lm

run: $(TARGET)
	./$(TARGET)

sanitize: $(SOURCES)
	$(CC) $(CFLAGS) $(SANITIZE_FLAGS) $(SOURCES) -o $(TARGET)

clean:
	rm -f $(TARGET) $(TEST_TARGET)
