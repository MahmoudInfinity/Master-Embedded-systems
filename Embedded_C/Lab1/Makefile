#@copywrite Mahmoud Elmowafy

CC=arm-none-eabi-
CFLAGS= -g -mcpu=arm926ej-s
INCS= -I .
LIBS=

all: versatilepb.bin
	@echo "=========Build is Done!======"
	
startup.o: 	 startup.s

	$(CC)as.exe $(CFLAGS) $< -o $@
	
uart.o: uart.c

	$(CC)gcc.exe -c $(CFLAGS)  $(INCS) $< -o $@

app.o: app.c 

	$(CC)gcc.exe -c  $(CFLAGS)  $(INCS) $< -o $@

Versatilepb.elf:  startup.o  uart.o app.o

	$(CC)ld.exe -T linker_script.ld $(LIBS) startup.o app.o uart.o -o $@ -MAP=map_file.map

versatilepb.bin: Versatilepb.elf

	$(CC)objcopy.exe -O binary $< $@

clean_all:
	rm *.o *.elf *.bin
clean:
		rm  *.elf *.bin