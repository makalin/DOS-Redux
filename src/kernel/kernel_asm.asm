; Kernel assembly support functions
[BITS 32]

global load_idt
global isr_common_stub

; Load IDT
load_idt:
    push ebp
    mov ebp, esp
    mov eax, [ebp + 8]  ; Get IDT pointer parameter
    lidt [eax]          ; Load IDT
    pop ebp
    ret

; Common Interrupt Service Routine stub
isr_common_stub:
    pusha               ; Push all registers
    
    ; Call C handler
    call interrupt_handler
    
    popa                ; Restore registers
    add esp, 8          ; Clean up stack
    iret                ; Return from interrupt 