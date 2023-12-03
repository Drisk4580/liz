#include "types.h"
#include "vga.h"
#include "asm.h"

static uint8_t
vga_colorentry(enum vga_color fg, enum vga_color bg)
{
  return fg | bg << 4;
}

static uint16_t
vga_entry(unsigned char uc, uint8_t color)
{
  return (uint16_t) uc | (uint16_t) color << 8;
}

static size_t row, column;
static uint8_t vga_color;
static volatile uint16_t *vga_mem;

static void
update_cursor()
{
  uint16_t cursorpos = row * VGA_WIDTH + column;
  outb(0x3D4, 14);
  outb(0x3D5, cursorpos >> 8);
  outb(0x3D4, 15);
  outb(0x3D5, cursorpos);
}

void
vgainit(enum vga_color fg, enum vga_color bg)
{
  vga_color = vga_colorentry(fg, bg);
  vga_mem = (uint16_t *)VGA_ADDRESS;
  if (vga_mem == NULL) {
    return;
  }

  vgaclear();
}

void
vgasetcolor(enum vga_color fg, enum vga_color bg)
{
  vga_color = vga_colorentry(fg, bg);
}

void
vgaclear()
{
  for (size_t y = 0; y < VGA_HEIGHT; y++) {
    for (size_t x = 0; x < VGA_WIDTH; x++) {
      vga_mem[y * VGA_WIDTH + x] = vga_entry(' ', vga_color);
    }
  }

  update_cursor();
}

void
vgaputc(char c)
{
  if (c == '\n') {
    column = 0;
    if (++row == VGA_HEIGHT) {
      row = VGA_HEIGHT - 1;
      vgascroll();
    }
  } else {
    vga_mem[row * VGA_WIDTH + column] = vga_entry(c, vga_color);
    if (++column == VGA_WIDTH) {
      column = 0;
      if (++row == VGA_HEIGHT) {
        row = 0;
        vgascroll();
      }
    }
  }

  update_cursor();
}


void
vgaputs(const char *str)
{
	for (unsigned int i = 0; str[i] != '\0'; i++)
		vgaputc(str[i]);
}

void
vgascroll()
{
	for (size_t y = 1; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			vga_mem[y * VGA_WIDTH + x] = vga_mem[(y - 1) * VGA_WIDTH + x];
		}
	}

	for (size_t x = 0; x < VGA_WIDTH; x++) {
		vga_mem[(VGA_HEIGHT - 1) * VGA_WIDTH + x] = vga_entry(' ', vga_color);
	}
}

