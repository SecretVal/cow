build:
	./make build

debug:
	./make build debug

make:
	gcc -o make make.c

all: make build

all_debug: make debug
