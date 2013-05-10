global _main							; making entry point visible to linker

global gdt_refresh
global read_pc
global idt_refresh
global halt_execution

;Also exports _irq[0-15]

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


halt_execution:
    hlt
    jmp halt_execution

read_pc:
	pop eax
	jmp eax

; Global Descriptor Table
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
extern idt_ptr
idt_refresh:
    lidt [idt_ptr]
    ret

; ISR Functions
%macro ISR 1
    global _isr%1
    _isr%1:
        cli
        push byte %1
        jmp 0x08:isr_router_call
%endmacro

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

ISR 0
ISR 1
ISR 2
ISR 3
ISR 4
ISR 5
ISR 6
ISR 7
ISR 8
ISR 9
ISR 10
ISR 11
ISR 12
ISR 13
ISR 14
ISR 15
ISR 16
ISR 17
ISR 18
ISR 19
ISR 20
ISR 21
ISR 22
ISR 23
ISR 24
ISR 25
ISR 26
ISR 27
ISR 28
ISR 29
ISR 30
ISR 31

extern isr_router
isr_router_call:
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
    call isr_router
    pop eax
    pop gs
    pop fs
    pop es
    pop ds
    popad
    add esp, 8
    iret

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
