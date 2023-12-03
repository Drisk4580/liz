#include "io.h"
#include "cpu.h"

unsigned long
strlen(const char *str)
{
  unsigned long len = 0;
  while (str[len] != '\0')
    len++;
  return len;
}

int
vsnprintf(char *s, size_t n, const char *format, va_list args)
{
  int count = 0;  // Keep track of the number of characters written.

  while (*format != '\0' && n > 1) {
    if (*format != '%') {
      *s++ = *format++;
      n--;
      count++;
    } else {
      format++;  // Skip '%'

      if (*format == 'd' || *format == 'i') {
        int value = va_arg(args, int);

        // Convert the integer to a string and write it to the buffer.
        char num_str[12];  // Sufficient for a 32-bit integer.
        int len = 0;
        int is_negative = 0;

        if (value < 0) {
            is_negative = 1;
            value = -value;
        }

        do {
          num_str[len++] = '0' + (value % 10);
          value /= 10;
        } while (value > 0);

        if (is_negative) {
          num_str[len++] = '-';
        }

        while (len > 0 && n > 1) {
          *s++ = num_str[--len];
          n--;
          count++;
        }
      } else if (*format == 's') {
        const char *str = va_arg(args, const char *);

        while (*str != '\0' && n > 1) {
          *s++ = *str++;
          n--;
          count++;
        }
      } else {
        // Unsupported format specifier, just copy it.
        *s++ = '%';
        *s++ = *format;
        n -= 2;
        count += 2;
      }

      format++;  // Move to the next character after the format specifier.
    }
  }

  // Ensure the result is null-terminated.
  if (n > 0) {
    *s = '\0';
  }

  return count;
}

int 
printf(const char *restrict format, ...)
{
  va_list ap;
  va_start(ap, format);
  
  char buf[1024];
  int count = vsnprintf(buf, 1024, format, ap);

  vgaputs(buf);

  va_end(ap);

  return count;
}

/// panics the operating system
_Noreturn void 
panic(const char *restrict format, ...)
{
  va_list ap;
  va_start(ap, format);
  
  char buf[1024];
  vsnprintf(buf, 1024, format, ap);

  vgaputs(buf);
  va_end(ap);

  asm ("cli");
  for (;;)
    asm ("hlt");
}
