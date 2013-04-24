global _main							; making entry point visible to linker
global magic							; we will use this in kmain
global mbd								; we will use this in kmain

extern kmain							; kmain is defined in kmain.c

; setting up the Multiboot header - see GRUB docs for details
MODULEALIGN equ 1<<0					; align loaded modules on page boundaries
MEMINFO		equ 1<<1					; provide memory map
FLAGS		equ MODULEALIGN | MEMINFO	; this is the Multiboot 'flag' field
MAGIC		equ 0x1BADB002				; 'magic number' lets bootloader find the header
CHECKSUM	equ -(MAGIC + FLAGS)

section .text

align 4
	dd MAGIC
	dd FLAGS
	dd CHECKSUM

; reserve initial kernel stack space
STACKSIZE equ 0x4000					; that's 16k.

_main:
	mov esp, stack + STACKSIZE			; set up the stack.
	mov [magic], eax					; Multiboot magic number
	mov [mbd], eax						; Multiboot info structure

    cli
	call kmain							; call kernel proper

	cli
.hang:
	hlt									; halt machine should kernel return
	jmp .hang

; Global Descriptor Table
global gdt_refresh
extern gdt_ptr
gdt_refresh:
    lgdt [gdt_ptr]
    jmp 0x08:gdt_refresh2
gdt_refresh2:
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    ret

section .bss

align 4
stack: 	resb STACKSIZE					; reserve 16k stack on doubleword boundary
magic: 	resd 1
mbd:	resd 1
