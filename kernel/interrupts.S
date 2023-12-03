.interrupt_stub:
        pusha                         /* Push general-purpose registers */
        push %ds                      /* Push data segment register */
        push %es                      /* Push extra segment register */


        call interrupt_dispatch

        pop %es                       /* Pop extra segment register */
        pop %ds                       /* Pop data segment register */
        popa                          /* Pop general-purpose registers */

        add $8, %esp                  /* Remove the vector number + error code */

        iret                          /* Return from interrupt */

.global vector_0_handler

.align 16
vector_0_handler:
        /* vector 0 has no error code */
        push $0
        /* the vector number */
        push $0
        jmp .interrupt_stub
