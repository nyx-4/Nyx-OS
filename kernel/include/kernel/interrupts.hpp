#ifndef _KERNEL_INTERRUPTS_HPP
#define _KERNEL_INTERRUPTS_HPP

#include <kernel/PS2_keyboard.hpp>
#include <kernel/framebuffer.hpp>
#include <stdint.h>

#define TRAP_GATE_FLAGS     0x8F // @brief 32-bit Trap gate in Ring 0
#define INT_GATE_FLAGS      0x8E // @brief 32-bit Interrupt gate in Ring 0
#define INT_GATE_USER_FLAGS 0xFF // @brief 32-bit Interrupt gate in Ring 3
#define PIC_1_CMD           0x20 // @brief Command port of master PIC
#define PIC_1_DATA          0x21 // @brief Data port of master PIC
#define PIC_2_CMD           0xA0 // @brief Command port of slave PIC
#define PIC_2_DATA          0xA1 // @brief Data port of slave PIC
#define NEW_IRQ_1           0x20 // @brief Master PIC will remap to 0x20-0x27
#define NEW_IRQ_2           0x28 // @brief Slave  PIC will remap to 0x28-0x2F
#define PIC_EOI             0x20 // @brief End of Interrupt Signal

typedef struct {
    uint16_t isr_low;    // The lower 16 bits of the ISR's address
    uint16_t kernel_cs;  // The GDT segment selector that the CPU will load into
                         // CS before calling the ISR
    uint8_t  reserved;   // Set to zero
    uint8_t  attributes; // Type and attributes; see the IDT page
    uint16_t isr_high;   // The higher 16 bits of the ISR's address
} __attribute__((packed)) idt_entry_t;

typedef struct {
    uint16_t limit; // The size of IDT table
    uint32_t base;  // The addr of IDT table
} __attribute__((packed)) idtr_t;

typedef struct {
    uint32_t eip; // Instruction pointer
    uint32_t cs;
    uint32_t eflags; // EFLAGS register
    uint32_t sp;     // Stack pointer
    uint32_t ss;
} __attribute__((packed)) int_frame_t;

extern "C" void syscall_dispatcher();

/**
 * @brief Default/General Exception handler, as a stub for more specific
 * Exception handler
 *
 * @param int_frame The pointer to Interrupt frame consisting of some registers
 * that are pushed on stack before calling this functions
 */
__attribute__((interrupt)) void def_excp_handler(int_frame_t *int_frame);
/**
 * @brief Default/General Exception handler, as a stub for more specific
 * Exception handler
 *
 * @param int_frame The pointer to Interrupt frame consisting of some registers
 * that are pushed on stack before calling this functions
 *
 * @param err_code The Error code that this exception generated, only some do.
 */
__attribute__((interrupt)) void def_excp_handler_e(int_frame_t *int_frame,
                                                   uint32_t     err_code);
/**
 * @brief Default/General Interrupt handler, as a stub for more specific
 * Interrupt handler
 *
 * @note Overload this function, and clear the IRQ mask to handle interrupts
 * properly
 *
 *
 * @param int_frame The pointer to Interrupt frame consisting of some registers
 * that are pushed on stack before calling this functions
 */
__attribute__((interrupt)) void def_int_handler(int_frame_t *int_frame);
extern "C" void                 idt_init(void);
extern "C" void                 idt_setup(void);
/**
 * @brief Set an entry in Interrupt Descriptor table at specified vector
 *
 * @param vector The index of interrupt, that this `isr` and `flags` describes
 * @param isr A 32 -bit value that represents the address of the entry point of
 * the Interrupt Service Routine.
 * @param flags 8-bit value:
 *
 * - `bit 0:` Present bit. Must be set (1) for the descriptor to be valid.
 *
 * - `bit 1-2:` defines the CPU Privilege Levels which are allowed to access
 * this interrupt via the INT instruction. Hardware interrupts ignore these bits
 *
 * - `bit 3:` Reserved, must be set to zero
 *
 * - `bit 4-7:` Gate Type, must be one of (0x5, 0x7 ,0x8 ,0xE, 0xF)
 */
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