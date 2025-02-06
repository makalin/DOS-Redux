; DOS Redux Bootloader
; Full Phase 1 implementation

[BITS 16]
[ORG 0x7C00]

KERNEL_OFFSET equ 0x1000    ; Kernel will be loaded at this address

start:
    ; Set up segments and stack
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7C00

    ; Save boot drive number
    mov [BOOT_DRIVE], dl

    ; Print welcome message
    mov si, MSG_WELCOME
    call print_string

    ; Load kernel from disk
    call load_kernel
    
    ; Get memory map
    call get_memory_map
    
    ; Switch to protected mode
    call switch_to_protected_mode
    
    jmp $

; Load kernel from disk
load_kernel:
    mov si, MSG_LOAD_KERNEL
    call print_string
    
    mov bx, KERNEL_OFFSET  ; Load kernel to this address
    mov dh, 15            ; Load 15 sectors
    mov dl, [BOOT_DRIVE]  ; From boot drive
    
    mov ah, 0x02         ; BIOS read sector function
    mov al, dh           ; Number of sectors
    mov ch, 0x00         ; Cylinder 0
    mov dh, 0x00         ; Head 0
    mov cl, 0x02         ; Start from sector 2
    
    int 0x13            ; BIOS interrupt
    jc disk_error       ; Jump if error (carry flag set)
    
    ret

; Get memory map using BIOS int 0x15, eax = 0xE820
get_memory_map:
    mov si, MSG_MEM_MAP
    call print_string
    
    mov di, 0x8000      ; Store memory map at 0x8000
    xor ebx, ebx        ; ebx must be 0 to start
    mov edx, 0x534D4150 ; 'SMAP'
    mov eax, 0xE820
    mov [es:di + 20], dword 1 ; force a valid ACPI entry
    mov ecx, 24         ; ask for 24 bytes
    int 0x15
    jc .failed          ; carry set on first call = unsupported
    
    ret

.failed:
    mov si, MSG_MEM_ERROR
    call print_string
    ret

; Switch to protected mode
switch_to_protected_mode:
    cli                 ; Disable interrupts
    lgdt [gdt_descriptor] ; Load GDT
    
    ; Enable protected mode
    mov eax, cr0
    or eax, 0x1
    mov cr0, eax
    
    ; Far jump to flush pipeline
    jmp CODE_SEG:init_pm

[BITS 32]
init_pm:
    ; Update segment registers
    mov ax, DATA_SEG
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    
    ; Set up new stack
    mov ebp, 0x90000
    mov esp, ebp
    
    ; Jump to kernel
    call KERNEL_OFFSET
    
    jmp $

; GDT
gdt_start:
    dq 0x0              ; Null descriptor
gdt_code:
    dw 0xFFFF           ; Limit
    dw 0x0              ; Base
    db 0x0              ; Base
    db 10011010b        ; Flags
    db 11001111b        ; Flags + Limit
    db 0x0              ; Base
gdt_data:
    dw 0xFFFF
    dw 0x0
    db 0x0
    db 10010010b
    db 11001111b
    db 0x0
gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start

CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start

; Variables
BOOT_DRIVE: db 0

; Messages
MSG_WELCOME: db 'DOS Redux Bootloader', 13, 10, 0
MSG_LOAD_KERNEL: db 'Loading kernel...', 13, 10, 0
MSG_MEM_MAP: db 'Getting memory map...', 13, 10, 0
MSG_MEM_ERROR: db 'Error getting memory map', 13, 10, 0

; Utility functions
print_string:
    lodsb
    or al, al
    jz .done
    mov ah, 0x0E
    int 0x10
    jmp print_string
.done:
    ret

disk_error:
    mov si, MSG_DISK_ERROR
    call print_string
    jmp $

MSG_DISK_ERROR: db 'Error loading kernel!', 13, 10, 0

times 510-($-$$) db 0
dw 0xAA55 