#include "idt.h"
#include "io.h"
#include "cpu.h"

/* support for the PIC (not APIC) */
/* this thing is legacy and should be replaced as soon as possible */

void
set_idt_entry(uint8_t vector, void *handler, uint8_t dpl)
{
  uint32_t handler_addr = (uint32_t) handler;

  struct idtentry_t *entry = &idt[vector];
  entry->address_low = handler_addr & 0xFFFF;
  entry->address_high = handler_addr >> 16;

  entry->selector = 0x8;
  entry->flags = 0b1110 | ((dpl & 0b11) << 5) | (1 << 7);

  entry->zero = 0;
}

void
load_idt(void *idt_addr)
{
  struct idtptr_t idt_reg;
  idt_reg.limit = 0xFFF;
  idt_reg.base = (uint32_t) idt_addr;

  uint32_t iptr[2];
  iptr[0] = * (uint32_t *) &idt_reg;
  iptr[1] = sizeof(struct idtptr_t) << 16;

  asm volatile ("lidt %0" :: "m"(*(uint8_t *) iptr));
}

extern uint8_t vector_0_handler[];

static const char *exception_names[32] = {
  "Divide By Zero Error",
  "Debug",
  "Non-Maskable Interrupt",
  "Breakpoint",
  "Overflow",
  "Bound Range Exceeded",
  "Invalid Opcode",
  "Device not available",
  "Double Fault",
  "Unused",
  "Invalid TSS",
  "Segment Not Present",
  "Stack-Segment Fault",
  "General Protection",
  "Page Fault",
  "RESERVED",
  "x87 FPU error",
  "Alignment Check",
  "Machine Check",
  "SIMD (SSE/AVX) error",
  "RESERVED",
  "RESERVED",
  "RESERVED",
  "RESERVED",
  "RESERVED",
  "RESERVED",
  "RESERVED",
  "RESERVED",
  "RESERVED",
  "RESERVED",
  "RESERVED",
  "RESERVED"
};

void
set_vector_handlers()
{
  for (size_t i = 0; i < 256; i++) {
    set_idt_entry(i, (void *) vector_0_handler + (i * 16), 0);
  }
}

void
interrupt_dispatch(struct cpu_state *ctx)
{
  int vector_num = ctx->eax ? 0 : 1;
  if (vector_num <= 32) {
    panic("%s\n", exception_names[vector_num]);
  }
}
