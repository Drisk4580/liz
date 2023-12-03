#ifndef IDT_H
#define IDT_H

#include "types.h"

struct idtentry_t {
  uint16_t address_low;
  uint16_t selector;
  uint8_t zero;
  uint8_t flags;
  uint16_t address_high;
} __attribute__((packed));

struct idtentry_t idt[256];

void set_idt_entry(uint8_t, void *, uint8_t);

struct idtptr_t {
  uint16_t limit;
  uint32_t base;
} __attribute__((packed));

void load_idt(void *idt_addr);

#endif // IDT_H
