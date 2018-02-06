
#Definitons
CC=arm-none-eabi-gcc

CFLAGS=-g -O1 -mcpu=cortex-m0 -mthumb

AS=arm-none-eabi-as

AFLAGS=-g -mcpu=cortex-m0 -mthumb

LD=arm-none-eabi-gcc

LFLAGS=-g -mcpu=cortex-m0 -mthumb --specs=rdimon.specs -lc -lrdimon

#Begin

test : HW1.elf
	qemu-system-arm  -machine versatilepb -cpu cortex-m3 -nographic -monitor null -serial null -semihosting -kernel HW1.elf

HW1.elf : test.o t1.o
	$(LD) $(LFLAGS) -o HW1.elf test.o t1.o

test.o : test.c
	$(CC) -c $(CFLAGS) test.c -o test.o

t1.o : t1.s
	$(AS) -c $(AFLAGS) t1.s -o t1.o

clean :
	rm *.o
