#ifndef GDT_H
#define GDT_H

#include "types.h"

struct gdtentry_t {
  uint16_t limit_low;
  uint16_t base_low;
  uint8_t type;
  uint8_t base_vhi;
  uint8_t flags_limit_high;
  uint8_t base_high;
} __attribute__((packed));

struct gdtentry_t setup_gdtentry(uint32_t, uint32_t, uint8_t);
uint32_t get_entrybase(struct gdtentry_t);
uint32_t get_entrylimit(struct gdtentry_t);

struct gdtptr_t {
  struct gdtentry_t null;
  struct gdtentry_t unused;
  struct gdtentry_t code;
  struct gdtentry_t data;
} __attribute__((packed));

struct gdtptr_t setup_gdt();
uint16_t get_codeentry(struct gdtptr_t);
uint16_t get_daraentry(struct gdtptr_t);

void load_gdt(struct gdtptr_t *);

#endif // GDT_H
