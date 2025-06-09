#ifndef _KERNEL_INTERRUPTS_HPP
#define _KERNEL_INTERRUPTS_HPP

#include <kernel/PS2_keyboard.hpp>
#include <kernel/framebuffer.hpp>
#include <stdint.h>

#define TRAP_GATE_FLAGS     0x8F
#define INT_GATE_FLAGS      0x8E
#define INT_GATE_USER_FLAGS 0xFF
#define PIC_1_CMD           0x20
#define PIC_1_DATA          0x21
#define PIC_2_CMD           0xA0
#define PIC_2_DATA          0xA1
#define NEW_IRQ_1           0x20
#define NEW_IRQ_2           0x28
#define PIC_EOI             0x20

typedef struct {
    uint16_t isr_low;    // The lower 16 bits of the ISR's address
    uint16_t kernel_cs;  // The GDT segment selector that the CPU will load into
                         // CS before calling the ISR
    uint8_t  reserved;   // Set to zero
    uint8_t  attributes; // Type and attributes; see the IDT page
    uint16_t isr_high;   // The higher 16 bits of the ISR's address
} __attribute__((packed)) idt_entry_t;

typedef struct {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed)) idtr_t;

typedef struct {
    uint32_t eip;
    uint32_t cs;
    uint32_t eflags;
    uint32_t sp;
    uint32_t ss;
} __attribute__((packed)) int_frame_t;

extern void *isr_stub_table[];

extern "C" void syscall_dispatcher();

__attribute__((interrupt)) void def_excp_handler(int_frame_t *int_frame);
__attribute__((interrupt)) void def_excp_handler_e(int_frame_t *, uint32_t);
__attribute__((interrupt)) void def_int_handler(int_frame_t *int_frame);
extern "C" void                 idt_init(void);
extern "C" void                 idt_setup(void);
void idt_set_descriptor(uint8_t vector, void *isr, uint8_t flags);

extern "C" void    outb(uint16_t port, uint8_t value);
extern "C" uint8_t inb(uint16_t port);
extern "C" void    io_wait();

void send_pic_eoi(uint8_t irq);
void disable_pic();
void set_irq_mask(uint8_t irq);
void clear_irq_mask(uint8_t irq);
void remap_pic();

#endif