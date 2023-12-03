#include "gdt.h"
#include "io.h"

struct gdtentry_t
setup_gdtentry(uint32_t base, uint32_t limit, uint8_t flags)
{
  struct gdtentry_t entry = {};
  uint8_t *target = (uint8_t*)&entry;

  if (limit <= 65536) {
    target[6] = 0x40;
  } else {
    if ((limit & 0xFFF) != 0xFFF) {
      limit = (limit >> 12) - 1;
    } else {
      limit = limit >> 12;
    }

    target[6] = 0xC0;
  }

  target[0] = limit & 0xFF;
  target[1] = limit >> 8 & 0xFF;
  target[6] |= (limit >> 16) & 0xF;


  target[2] = base & 0xFF;
  target[3] = (base >> 8) & 0xFF;
  target[4] = (limit >> 16) & 0xFF;
  target[7] = (limit >> 24) & 0xFF;

  target[5] = flags;

  return entry;
}

uint32_t
get_entrybase(struct gdtentry_t entry)
{
  uint8_t *target = (uint8_t *) &entry;
  uint32_t result = target[7];

  result = (result << 8) + target[4];
  result = (result << 8) + target[3];
  result = (result << 8) + target[2];

  return result;
}

uint32_t
get_entrylimit(struct gdtentry_t entry)
{
  uint8_t *target = (uint8_t *) &entry;
  uint32_t result = target[6] & 0xF;

  result = (result << 8) + target[1];
  result = (result << 8) + target[0];

  if ((target[6] & 0xC0) == 0xC0) {
    result = (result << 12) | 0xFFF;
  }

  return result;
}

struct gdtptr_t
setup_gdt()
{
  struct gdtptr_t gdt = { 
    setup_gdtentry(0, 0, 0),
    setup_gdtentry(0, 0, 0),
    setup_gdtentry(0, 64 * 1024 * 1024, 0x9A),
    setup_gdtentry(0, 64 * 1024 * 1024, 0x92),
  };
  
  return gdt;
}

void
load_gdt(struct gdtptr_t *gdt)
{
	uint32_t i[2];
	i[0] = *(uint32_t *) gdt;
	i[1] = sizeof(struct gdtptr_t) << 16;

	asm ("lgdt %0" : : "m"(*(uint8_t *) i));
}

uint16_t
get_codeentry(struct gdtptr_t gdt)
{
  return (uint8_t *)&gdt.code - (uint8_t *) &gdt;
}

uint16_t
get_dataentry(struct gdtptr_t gdt)
{
  return (uint8_t *)&gdt.data - (uint8_t *) &gdt;
}
