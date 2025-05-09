.intel_syntax noprefix

# Constants for GDT segment offsets
.section .text
.equ CODE_SEG,  GDT_kcode -  GDT_start
.equ DATA_SEG,  GDT_kdata -  GDT_start

.global setupGDT
setupGDT:
    # Disable interrupts and load GDT
    cli
    lgdt GDT_descriptor

    # Enable Protected Mode
    mov eax, cr0
    or eax, 1
    mov cr0, eax

    # reloadSegments and Far jump to Protected Mode
    jmp reloadSegments
    jmp CODE_SEG:start_protected_mode

reloadSegments:
   mov   ax, DATA_SEG      # Load data segment selector
   mov   ds, ax            # Set DS register
   mov   es, ax            # Set ES register
   mov   fs, ax            # Set FS register
   mov   gs, ax            # Set GS register
   mov   ss, ax            # Set SS register
   ret                     # Return

# GDT definition
.section .data
GDT_start:
GDT_null:
    .long 0x0
    .long 0x0
GDT_kcode:        # Code segment for kernel
    .word 0xffff
    .word 0x0
    .byte 0x0
    .byte 0x9A    # 10011010: present, ring 0, code, executable, readable
    .byte 0xCF    # 11001111: 4KB granularity, 32-bit, limit 16-19
    .byte 0x0
GDT_kdata:        # Data segment for kernel
    .word 0xffff
    .word 0x0
    .byte 0x0
    .byte 0x92    # 10010010: present, ring 0, data, writable
    .byte 0xCF    # 11001111: 4KB granularity, 32-bit, limit 16-19
    .byte 0x0
GDT_ucode:        # Code segment for User space
    .word 0xffff
    .word 0x0
    .byte 0x0
    .byte 0xFA    # 11111010: present, ring 3, code, executable, readable
    .byte 0xCF    # 11001111: 4KB granularity, 32-bit, limit 16-19
    .byte 0x0
GDT_udata:        # Data segment for User space
    .word 0xffff
    .word 0x0
    .byte 0x0
    .byte 0xF2    # 11110010: present, ring 3, data, writable
    .byte 0xCF    # 11001111: 4KB granularity, 32-bit, limit 16-19
    .byte 0x0
GDT_tss:          # Task State Segment (Incomplete, set base and limit properly)
    .word 0x0000
    .word 0x0
    .byte 0x0
    .byte 0x89    # 10001001: present, ring 0, system, 32-bit TSS (Available)
    .byte 0x4F    # 01001111: 1B granularity, 32-bit, limit 16-19
    .byte 0x0
GDT_end:

GDT_descriptor:
    .word GDT_end - GDT_start - 1
    .long GDT_start

# Protected Mode code
.section .text
.code32
start_protected_mode:

