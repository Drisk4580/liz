#ifndef CPU_H
#define CPU_H

#include "types.h"

struct cpu_state {
    // General-purpose registers pushed by the interrupt handler
    uint32_t edi;
    uint32_t esi;
    uint32_t ebp;
    uint32_t esp;
    uint32_t ebx;
    uint32_t edx;
    uint32_t ecx;
    uint32_t eax;

    // Segment registers pushed by the interrupt handler
    uint32_t iret_ss;
    uint32_t iret_rsp;
    uint32_t iret_flags;
    uint32_t iret_cs;
    uint32_t iret_rip;

    // Registers pushed by the CPU during an interrupt
    uint32_t error_code;
    uint32_t vector_number;
};


#endif // CPU_H
