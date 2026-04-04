#@copywrite Mahmoud Elmowafy

	
startup.o: 	 startup.s

	arm-none-eabi-as.exe -mcpu=arm926ej-s startup.s -o startup.o
	
uart.o: uart.c

	arm-none-eabi-gcc.exe -c -mcpu=arm926ej-s -nostdlib -ffreestanding -I . uart.c -o uart.o

app.o: app.c 

	arm-none-eabi-gcc.exe -c -mcpu=arm926ej-s -nostdlib -ffreestanding -I . app.c -o app.o

Versatilepb.elf:  startup.o  uart.o app.o

	arm-none-eabi-ld.exe -T linker_script.ld startup.o app.o uart.o -o Versatilepb.elf -MAP=map_file.map

versatilepb.bin: Versatilepb.elf

	arm-none-eabi-objcopy.exe -O binary Versatilepb.elf Versatilepb.bin

clean_all:
	rm *.o *.elf *.bin
clean:
		rm  *.elf *.bin