SHELL := /bin/bash

disassember: main.c
	gcc -w -o disassember main.c 

ARG := $(shell cat test.hex)

test:
	gcc -w -o disassember main.c
	arm-none-eabi-as test.asm -o test.o
	arm-none-eabi-objcopy test.o -O binary --only-section=.text test.bin
	hexdump -x test.bin |sed -e 's/^[^ ]*//' > test.hex
	./disassember $(ARG)
	diff test.asm test.out > test.diff
