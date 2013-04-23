SYSTEM = $(patsubst %.c,%.o,$(wildcard kernel/system/*.c))
VIDEO = $(patsubst %.c,%.o,$(wildcard kernel/video/*.c))

SUBMODULES = ${SYSTEM} ${VIDEO}

all: bin/floppy.img bochsrc.txt

bin/floppy.img: image/kernel.bin image/pad
	@echo "Constructing kernel image..."
	@cat image/stage1 image/stage2 image/pad image/kernel.bin > image/floppy.img
	@echo "Kernel image created."
	@echo "Copying kernel image to ./bin/..."
	@mkdir -p bin
	@cp bochsrc.txt bin/bochsrc.txt
	@cp image/floppy.img bin/floppy.img
	@echo "Copied kernel image to ./bin/floppy.img"
	@echo "Kernel image stored in ./bin/floppy.img, ready to boot"

image/kernel.bin: kernel/start.o kernel/kmain.o ${SUBMODULES} kernel/link.ld
	@echo "Linking kernel source files..."
	@ld -T kernel/link.ld -o image/kernel.bin kernel/*.o ${SUBMODULES}
	@echo "Linked kernel."

image/pad:
	@echo "Generating pad file..."
	@dd if=/dev/zero of=image/pad bs=1 count=750 2> /dev/null
	@echo "Pad file generated."

GCCWARN = -Wall -Wextra -Werror
CFLAGS = $(GCCWARN) -m32 -nostdlib -nostartfiles -nodefaultlibs -I./include

%.o: %.c
	@gcc $(CFLAGS) -c -o $@ $<

kernel/start.o: kernel/start.s
	@echo "Assembling kernel loader code..."
	@nasm -f elf -o kernel/start.o kernel/start.s
	@echo "Kernel loader code assembled."

 ###############
 #    clean    #
 ###############

clean-temp:
	@echo "Cleaning temporary files..."
	@rm -f core *~ image/pad */*~ */*/*~
	@echo "Cleaned temporary files."

clean-objects:
	@echo "Cleaning object files..."
	@rm -f kernel/*.o ${SUBMODULES}
	@echo "Cleaned object files."

clean-binaries:
	@echo "Cleaning binary files..."
	@rm -f image/floppy.img image/kernel.bin
	@rm -rf bin
	@echo "Cleaned binary files."

clean: clean-temp clean-objects clean-binaries
	@echo "Finished cleaning."
