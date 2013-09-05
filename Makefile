# AVR Makefile

# what do you want to call your binary
PROG=libarduino

# please uncomment the CPU that is used on your arduino
CPU=atmega328p
#CPU=atmega168

# please uncomment the speed that corresponds to your arduino (normally 16MHz)
F_CPU=16000000
#F_CPU=8000000

#############################################################################
# no additional config needed below (except perpapse the 'make install')
#############################################################################

CFLAGS= -g -Os -Wall -Wstrict-prototypes -Wa,-ahlms=$(PROG).lst -mmcu=$(CPU) -DF_CPU=$(F_CPU)

SRC = uart.c ir.c pwm.c adc.c gpio.c  
OBJ = $(SRC:.c=.o)

# default target when "make" is run w/o arguments
all: $(PROG).a

# compile .c into .o
%.o: %.c
	avr-gcc -c $(CFLAGS) -I. $*.c

# link up *.o to create the archive file
$(PROG).a: $(OBJ)
	avr-ar rcs $(PROG).a $(OBJ)

# command to clean up junk (no source files) (invoked by "make clean")
clean:
	rm -f *.o *.rom *.elf *.a *.map *~ *.lst

