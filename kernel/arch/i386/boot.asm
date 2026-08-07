; https://wiki.osdev.org/Bare_Bones_with_NASM
; https://wiki.osdev.org/Meaty_Skeleton#kernel/arch/i386/boot.S
; https://gitlab.com/sortie/meaty-skeleton/-/blob/e7b93d8c/kernel/arch/i386/boot.S
; https://www.gnu.org/software/grub/manual/multiboot2/multiboot.html
; https://cgit.git.savannah.gnu.org/cgit/grub.git/tree/doc/multiboot2.h?h=multiboot2

MAGIC             equ  0xE85250D6
ARCHITECTURE      equ  0 ; i386
MULTIBOOT2_LENGTH equ  (multiboot2.MB2_END - multiboot2)
CHECKSUM          equ  -(MAGIC + ARCHITECTURE + MULTIBOOT2_LENGTH)

; The value zero indicates that the OS image has no preference.
PREF_WIDTH        equ  0  ; number of the columns (or pixels in a graphics mode)
PREF_HEIGHT       equ  0  ; number of the lines (or pixels in a graphics mode)
PREF_DEPTH        equ  32 ; number of bits per pixel in a graphics mode

OPTIONAL_TAG      equ  0
REQUIRED_TAG      equ  1


section .multiboot2
align 8
multiboot2:
    ; Section '3.1.1 The layout of Multiboot2 header'
    dd MAGIC
    dd ARCHITECTURE
    dd MULTIBOOT2_LENGTH
    dd CHECKSUM


; Request the bootloader to provide the 'multiboot2 information structures' (mbi)
; Section '3.1.4 Multiboot2 information request'
.Lmbi_start:
    align 8
    dw 1  ; multiboot2 information request tag is 1
    dw REQUIRED_TAG
    dd multiboot2.Lmbi_end - multiboot2.Lmbi_start  ; size of mbi tag

    ; Section '3.6 Boot information'
    dd 1  ; Boot command line
    dd 3  ; Modules
    dd 6  ; Memory map
    dd 8  ; Framebuffer info
.Lmbi_end:

; Request a 32-bit framebuffer if available.
; Sextion '3.1.10 The framebuffer tag of Multiboot2 header'
.Lfb_start:
    align 8
    dw 5  ; framebuffer tag is 5
    dw OPTIONAL_TAG
    dd multiboot2.Lfb_end - multiboot2.Lfb_start  ; size of fb tag

    dd PREF_WIDTH
    dd PREF_HEIGHT
    dd PREF_DEPTH
.Lfb_end:


; Section '3.1.11 Module alignment tag' and 
; a NULL tag to mark end of multiboot2 tags
.Lnull_start:
    align 8
    dw 6  ; Module alignment tag
    dw REQUIRED_TAG
    dd 8  ; size is 8 bytes

    align 8
    dw 0  ; NULL tag
    dw OPTIONAL_TAG
    dd 8  ; size is 8 bytes
.Lnull_end:

; multiboot2 protocol end here
.MB2_END:



section .bss
align 16
stack_bottom:
    resb 16384 ; 16 KiB is reserved for stack
stack_top:

; kernel's entry point
section .text
global _start:function (_start.end - _start)
_start:
    mov esp, stack_top
    sub esp, 8 ; 16-byte align stack for call

    ; debug
    push ebx
    push eax

    ; execute the kernel
    extern kernel_early
    call kernel_early

    ; hang if the kernel returns (which it shouldn't)
    cli
.hang:  hlt
    jmp .hang

.end:



; Misc. assembly functions here
global setup_gdt:function (setup_gdt.end - setup_gdt)
setup_gdt:
    cli ; clear interrupts

    ; setup_gdt(size, offset)
    mov ax, [esp + 4]
    mov [gdtr], ax      ; set size
    mov eax, [esp + 8]
    mov [gdtr + 2], eax ; set offset

    lgdt [gdtr]

    jmp  0x08:.reload_CS ; 0x08 is a stand-in for your code segment

.reload_CS:
    mov ax, 0x10 ; 0x10 is a stand-in for your data segment
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    ret

.end:


section .data:
gdtr:
    dw 0 ; size   (limit)
    dd 0 ; offset (base address)
