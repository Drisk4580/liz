#include "gdt.h"
#include "vga.h"
#include "io.h"

/* this gets called in boot.S */
void
kentry()
{
  vgainit(VGA_COLOR_LIGHT_GRAY, VGA_COLOR_BLACK);
  printf("%s ", "$");

  struct gdtptr_t gdt = setup_gdt();
  load_gdt(&gdt);
  panic("IDT failure");
}
