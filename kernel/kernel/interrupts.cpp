#include <kernel/interrupts.hpp>

__attribute__((aligned(0x10))) static idt_entry_t
    idt[256]; // Create an array of IDT entries; aligned for performance
static idtr_t idtr;

void def_excp_handler(int_frame_t *int_frame) {
    FBuf::terminal_writestring("DEFAULT EXCEPTION HANDLER -- NO ERROR CODE");
    int_frame->eip++;
}
void def_excp_handler_e(int_frame_t *int_frame, uint32_t err_code) {
    FBuf::terminal_writestring("DEFAULT EXCEPTION HANDLER -- ERROR CODE: ");
    FBuf::terminal_writehex(err_code);
    int_frame->eip++;
}
void def_int_handler(int_frame_t *int_frame) {
    FBuf::terminal_writestring("DEFAULT INTERRUPT HANDLER");
}

void idt_set_descriptor(uint8_t vector, void *isr, uint8_t flags) {
    idt_entry_t *descriptor = &idt[vector];

    descriptor->isr_low   = (uint32_t)isr & 0xFFFF;
    descriptor->kernel_cs = 0x08; // this value can be whatever offset your
    // kernel code selector is in your GDT
    descriptor->reserved   = 0;
    descriptor->attributes = flags;
    descriptor->isr_high   = (uint32_t)isr >> 16;
}

void idt_init() {
    idtr.base       = (uintptr_t)&idt[0];
    idtr.limit      = (uint16_t)sizeof(idt_entry_t) * 256 - 1;
    uint16_t vector = 0;
    for (; vector < 32; vector++) {
        if (vector == 8 || vector == 10 || vector == 11 || vector == 12 ||
            vector == 13 || vector == 14 || vector == 17 || vector == 21)
            idt_set_descriptor(vector, (void *)def_excp_handler_e,
                               TRAP_GATE_FLAGS);
        else
            idt_set_descriptor(vector, (void *)def_excp_handler,
                               TRAP_GATE_FLAGS);
    }

    for (; vector < 256; vector++)
        idt_set_descriptor(vector, (void *)def_int_handler, INT_GATE_FLAGS);

    idt_setup(); // Setup precise IDT handlers.

    clear_irq_mask(1);
    clear_irq_mask(2);

    __asm__ volatile("lidt %0" : : "m"(idtr)); // load the new IDT
    __asm__ volatile("sti");                   // set the interrupt flag
    return;
}

void outb(uint16_t port, uint8_t value) {
    __asm__ __volatile__("outb %0, %1" : : "a"(value), "Nd"(port));
}
uint8_t inb(uint16_t port) {
    uint8_t ret_val = 0;
    __asm__ __volatile__("inb %1, %0" : "=a"(ret_val) : "Nd"(port));
    return ret_val;
}
void io_wait() { __asm__ __volatile__("outb %%al, $0x80" : : "a"(0)); }

void send_pic_eoi(uint8_t irq) {
    if (irq >= 8) outb(PIC_2_CMD, PIC_EOI);
    outb(PIC_1_CMD, PIC_EOI);
}
void disable_pic() {
    outb(PIC_2_DATA, 0xFF);
    outb(PIC_1_DATA, 0xFF);
}
void set_irq_mask(uint8_t irq) {
    uint16_t port;
    if (irq < 8) port = PIC_1_DATA;
    else {
        irq  -= 8;
        port  = PIC_2_DATA;
    }
    // Get the current mask and clear the irq-th bit
    outb(port, inb(port) | (1 << irq));
}
void clear_irq_mask(uint8_t irq) {
    uint16_t port;
    if (irq < 8) port = PIC_1_DATA;
    else {
        irq  -= 8;
        port  = PIC_2_DATA;
    }
    // Get the current mask and clear the irq-th bit
    outb(port, inb(port) & ~(1 << irq));
}
void remap_pic() {
    // Save PIC mask to restore later, when done remapping
    uint8_t pic_1_mask = inb(PIC_1_DATA), pic_2_mask = inb(PIC_2_DATA);

    // ICW 1 (Initialization Control Word)
    // bit 0: Send upto ICW 4
    // bit 4: initalize PIC
    outb(PIC_1_CMD, 0b00010001);
    io_wait();
    outb(PIC_2_CMD, 0b00010001);
    io_wait();

    // ICW 2 (where to map base interrupt in IDT)
    outb(PIC_1_DATA, NEW_IRQ_1);
    io_wait();
    outb(PIC_2_DATA, NEW_IRQ_2);
    io_wait();

    // ICW 3 (where to map PIC2 to IRQ line in PIC 1)
    outb(PIC_1_DATA, 0x4); // bit 2: means IRQ pin#2
    io_wait();
    outb(PIC_2_DATA, 2); // binary # for IRQ in PIC1
    io_wait();

    // ICW 4 (Initialization Control Word) - Part 4
    outb(PIC_1_DATA, 0x1);
    io_wait();
    outb(PIC_2_DATA, 0x1);
    io_wait();

    // Restore saved PIC mask.
    outb(PIC_1_DATA, pic_1_mask);
    outb(PIC_2_DATA, pic_2_mask);
}

// Define all precise interrupts here.
__attribute__((interrupt)) void
div_by_0_handler(volatile int_frame_t *int_frame) {
    FBuf::terminal_writestring("DIVIDE BY ZERO -- EXCEPTION HANDLER");
    int_frame->eip++;
}
void general_protextion_fault(int_frame_t *int_frame, uint32_t err_code) {
    FBuf::terminal_writestring(
        "GENERAL PROTECTION FAULT HANDLER -- ERROR CODE: ");
    FBuf::terminal_writehex(err_code);
    int_frame->eip++;
}
__attribute__((interrupt)) void
keyboard_handler(volatile int_frame_t *int_frame) {
    handle_key(inb(0x60));
    send_pic_eoi(1);
}

// This function will setup interrupts more precisely.
void idt_setup() {
    disable_pic();
    remap_pic();

    idt_set_descriptor(0x00, (void *)div_by_0_handler, TRAP_GATE_FLAGS);
    idt_set_descriptor(0x0D, (void *)general_protextion_fault, TRAP_GATE_FLAGS);
    idt_set_descriptor(0x21, (void *)keyboard_handler, INT_GATE_FLAGS);
    idt_set_descriptor(0x80, (void *)syscall_dispatcher, INT_GATE_FLAGS);
}
