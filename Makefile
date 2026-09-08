CC = gcc
CFLAGS = -Wall -Wextra -Wno-unused -Wpedantic \
				 -I./include \
         -I./src/core \
				 -I./src/numerics \
				 -I./src/models
SANITIZE_FLAGS = -fsanitize=address,undefined -g

SOURCES = src/main.c \
						include/draw.c\
						src/core/vmath.c \
						src/core/matrix.c \
						src/models/sir.c \
						src/numerics/solver.c
TARGET = dynsys
TEST_SOURCES = $(wildcard src/test/*.c)
TEST_TARGET = src/test/test_main


.PHONY: all run test sanitize clean plot

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES) -o $(TARGET)

test: src/test/test_main
	./src/test/test_main

src/test/test_main: $(TEST_SOURCES) src/core/vmath.c src/core/matrix.c
	$(CC) $(CFLAGS) $^ -o $@ -lm

run: $(TARGET)
	./$(TARGET)

sanitize: $(SOURCES)
	$(CC) $(CFLAGS) $(SANITIZE_FLAGS) $(SOURCES) -o $(TARGET)

clean:
	rm -f $(TARGET) $(TEST_TARGET) ./output.ppm

plot: run
	feh output.ppm &
