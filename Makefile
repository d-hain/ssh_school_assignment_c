CC = clang
CC_FLAGS = -Wall -Wextra -Werror -Wpedantic -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function
CC_LINK_FLAGS = -lssh

.PHONY: all run clean
all: run

run: build
	./target/main.o

build: src/main.c
	$(CC) -o target/main.o src/main.c $(CC_FLAGS) $(CC_LINK_FLAGS)
	chmod u+x target/main.o

clean:
	rm -r target/*
