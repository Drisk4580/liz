#ifndef ASM_H
#define ASM_H

#include "types.h"

inline void
outb(uint16_t port, uint8_t val)
{
	asm volatile ("outb %0, %1" :: "a" (val), "d" (port));
}

inline uint8_t
inb(uint16_t port)
{
	uint8_t val;
	asm volatile ("inb %1, %0" : "=a" (val) : "d" (port));
	return val;
}

#endif // ASM_H
