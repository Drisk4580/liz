#ifndef IO_H
#define IO_H

#include "types.h"
#include "vga.h"

typedef char *va_list;

#define va_start(ap, param) ((ap) = (va_list)(&(param) + 1))

#define va_arg(ap, type) \
  (ap += sizeof(type), *((type *)(ap - sizeof(type))))

#define va_end(ap) ((void) 0)

int vsnprintf(char *, size_t, const char *, va_list);

unsigned long strlen(const char *);

int printf(const char *restrict, ...);
_Noreturn void panic(const char *restrict, ...);

#endif // IO_H
