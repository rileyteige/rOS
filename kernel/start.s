global _main							; making entry point visible to linker

extern kmain							; kmain is defined in kmain.c

; setting up the Multiboot header - see GRUB docs for details
MODULEALIGN equ 1<<0					; align loaded modules on page boundaries
MEMINFO		equ 1<<1					; provide memory map
FLAGS		equ MODULEALIGN | MEMINFO	; this is the Multiboot 'flag' field
MAGIC		equ 0x1BADB002				; 'magic number' lets bootloader find the header
CHECKSUM	equ -(MAGIC + FLAGS)

section .text

bits 32
align 4
	dd MAGIC
	dd FLAGS
	dd CHECKSUM

; reserve initial kernel stack space
STACKSIZE equ 0x4000					; that's 16k.

_main:
	mov esp, stack + STACKSIZE			; set up the stack.
	push eax                            ; Magic
	push ebx                            ; Multiboot header

    cli
	call kmain							; call kernel proper

	cli
.hang:
	hlt									; halt machine should kernel return
	jmp .hang

global halt_execution
halt_execution:
    hlt
    jmp halt_execution

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

; Interrupt Descriptor Table
global idt_refresh
extern idt_ptr
idt_refresh:
    lidt [idt_ptr]
    ret

; IRQ Functions
%macro IRQ 2
    global _irq%1
    _irq%1:
        cli
        push byte 0
        push byte %2
        jmp 0x08:irq_router_call
%endmacro

IRQ 0, 32
IRQ 1, 33
IRQ 2, 34
IRQ 3, 35
IRQ 4, 36
IRQ 5, 37
IRQ 6, 38
IRQ 7, 39
IRQ 8, 40
IRQ 9, 41
IRQ 10, 42
IRQ 11, 43
IRQ 12, 44
IRQ 13, 45
IRQ 14, 46
IRQ 15, 47

extern irq_router
irq_router_call:
    pushad
    push ds
    push es
    push fs
    push gs
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov eax, esp
    push eax
    mov eax, irq_router
    call eax
    pop eax
    pop gs
    pop fs
    pop es
    pop ds
    popad
    add esp, 8
    iret

section .bss

align 4
stack: 	resb STACKSIZE					; reserve 16k stack on doubleword boundary
magic: 	resd 1
mbd:	resd 1
