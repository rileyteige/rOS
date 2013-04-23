SYSTEM = $(patsubst %.c,%.o,$(wildcard kernel/system/*.c))
VIDEO = $(patsubst %.c,%.o,$(wildcard kernel/video/*.c))

SUBMODULES = ${SYSTEM} ${VIDEO}

all: bin/floppy.img bochsrc.txt

bin/floppy.img: image/kernel.bin image/pad
	@echo "Constructing kernel image..."
	@cat image/stage1 image/stage2 image/pad image/kernel.bin > image/floppy.img
	@echo "Copying kernel image to ./bin/..."
	@mkdir -p bin
	@cp bochsrc.txt bin/bochsrc.txt
	@echo "Copying bochs config file to ./bin/..."
	@cp image/floppy.img bin/floppy.img
	@echo "Kernel image stored in ./bin/floppy.img, ready to boot"

LDFLAGS = -m elf_i386

image/kernel.bin: kernel/start.o kernel/kmain.o ${SUBMODULES} kernel/link.ld
	@echo "Linking kernel source files..."
	@ld -T kernel/link.ld ${LDFLAGS} -o image/kernel.bin kernel/*.o ${SUBMODULES}

image/pad:
	@echo "Generating pad file..."
	@dd if=/dev/zero of=image/pad bs=1 count=750 2> /dev/null

GCCWARN = -Wall -Wextra -Werror
CFLAGS = $(GCCWARN) -m32 -nostdlib -nostartfiles -nodefaultlibs -I./kernel/include

%.o: %.c
	@gcc $(CFLAGS) -c -o $@ $<

kernel/start.o: kernel/start.s
	@echo "Assembling kernel loader code..."
	@nasm -f elf -o kernel/start.o kernel/start.s

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
