#@copywrite Mahmoud Elmowafy

CC=arm-none-eabi-
CFLAGS= -g -mcpu=arm926ej-s
INCS= -I .
LIBS=
SRC=$(wildcard *.c)
OBJ=$(SRC:.c=.o)
As=$(wildcard *.s)
AsOBJ=$(As:.s=.o)
ProjectName=versatilepb


all: versatilepb.bin
	@echo "=========Build is Done!======"
	
startup.o: 	 startup.s

	$(CC)as.exe $(CFLAGS) $< -o $@
	

%.o: %.c 

	$(CC)gcc.exe -c  $(CFLAGS)  $(INCS) $< -o $@

$(ProjectName).elf:  $(OBJ) $(AsOBJ)

	$(CC)ld.exe -T linker_script.ld $(LIBS) $(OBJ) $(AsOBJ) -o $@ -MAP=map_file.map

$(ProjectName).bin: $(ProjectName).elf

	$(CC)objcopy.exe -O binary $< $@

clean_all:
	rm *.o *.elf *.bin
clean:
		rm  *.elf *.bin