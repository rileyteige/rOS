 # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
 #                                                                   #
 # rOS - A scratch operating system.                                 #
 #                                                                   #
 #                                                                   #
 # Author: Riley P. Teige                                            #
 # Start Date: 4/20/2013                                             #
 #                                                                   #
 # This is just a scratch OS I am writing in C, because I like to    #
 # get my hands dirty. I figure I'll learn a thing or two about      #
 # both OS development/design as well as large-scale project manage- #
 # ment in the process.                                              #
 #                                                                   #
 # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

all: bin/floppy.img bochsrc.txt

 ################
 #  boot image  #
 ################

bin/floppy.img: image/kernel.bin image/pad
	@echo "Constructing kernel image..."
	@cat image/stage1 image/stage2 image/pad image/kernel.bin > image/floppy.img
	@echo "Copying kernel image to ./bin/..."
	@mkdir -p bin
	@cp bochsrc.txt bin/bochsrc.txt
	@echo "Copying bochs config file to ./bin/..."
	@cp image/floppy.img bin/floppy.img
	@echo "Kernel image stored in ./bin/floppy.img, ready to boot"

image/pad:
	@dd if=/dev/zero of=image/pad bs=1 count=750 2> /dev/null

 ################
 #    kernel    #
 ################

SYSTEM = $(patsubst %.c,%.o,$(wildcard kernel/system/*.c))
VIDEO = $(patsubst %.c,%.o,$(wildcard kernel/video/*.c))
CPU = $(patsubst %.c,%.o,$(wildcard kernel/cpu/*.c))
DEVICES = $(patsubst %.c,%.o,$(wildcard kernel/devices/*.c))
MEMORY = $(patsubst %.c,%.o,$(wildcard kernel/memory/*.c))
UTILITIES = $(patsubst %.c,%.o,$(wildcard kernel/util/*.c))

SUBMODULES = ${SYSTEM} ${VIDEO} ${CPU} ${DEVICES} ${MEMORY} ${UTILITIES}

LDFLAGS = -m elf_i386

image/kernel.bin: ${SUBMODULES} kernel/kmain.o kernel/start.o kernel/link.ld
	@echo "Linking kernel source files..."
	@ld -T kernel/link.ld ${LDFLAGS} -o image/kernel.bin kernel/*.o ${SUBMODULES}

ASMFLAGS = -f elf

kernel/start.o: kernel/start.s
	@echo "Assembling kernel bootstrapper..."
	@nasm ${ASMFLAGS} -o kernel/start.o kernel/start.s

GCCWARN = -Wall -Wextra -Werror
CFLAGS = ${GCCWARN} -m32 -nostdlib -Wno-unused-parameter -nostartfiles -nodefaultlibs -I./kernel/include

%.o: %.c
	@echo "Compiling $<..."
	@gcc ${CFLAGS} -c -o $@ $<

 ###############
 #    clean    #
 ###############

clean-temp:
	@echo "Cleaning temporary files..."
	@rm -f core image/pad $(shell find . -name "*~")

clean-objects:
	@echo "Cleaning object files..."
	@rm -f kernel/*.o ${SUBMODULES}

clean-binaries:
	@echo "Cleaning binary files..."
	@rm -f image/floppy.img image/kernel.bin
	@rm -rf bin

clean: clean-temp clean-objects clean-binaries
	@echo "Finished cleaning."
