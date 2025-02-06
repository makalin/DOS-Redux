/**
 * DOS Redux Kernel
 * Full Phase 1 implementation
 */

#include <stdint.h>
#include "memory/memory.h"

// Interrupt Descriptor Table structure
struct idt_entry {
    uint16_t base_low;
    uint16_t selector;
    uint8_t zero;
    uint8_t flags;
    uint16_t base_high;
} __attribute__((packed));

struct idt_ptr {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

#define IDT_ENTRIES 256
static struct idt_entry idt[IDT_ENTRIES];
static struct idt_ptr idtp;

// External assembly functions
extern void load_idt(struct idt_ptr* ptr);
extern void isr_common_stub(void);

// Initialize IDT
static void init_idt(void) {
    // Set up IDT pointer
    idtp.limit = (sizeof(struct idt_entry) * IDT_ENTRIES) - 1;
    idtp.base = (uint32_t)&idt;

    // Clear IDT
    for (int i = 0; i < IDT_ENTRIES; i++) {
        idt[i].base_low = 0;
        idt[i].selector = 0;
        idt[i].zero = 0;
        idt[i].flags = 0;
        idt[i].base_high = 0;
    }

    // Load IDT
    load_idt(&idtp);
}

// Initialize memory management
static void init_memory(void) {
    // Get memory map from bootloader
    memory_region_t* mem_map = (memory_region_t*)0x8000;
    uint32_t total_mem = 0;

    // Calculate total available memory
    for (int i = 0; mem_map[i].type != 0; i++) {
        if (mem_map[i].type == 1) { // Available memory
            total_mem += mem_map[i].size;
        }
    }

    // Initialize memory manager
    init_memory_manager();
}

void kernel_main(void) {
    // Initialize core components
    init_idt();
    init_memory();
    
    // Initialize display
    init_video();
    
    kprintf("DOS Redux Kernel\n");
    kprintf("Memory: %d KB\n", get_total_memory() / 1024);
    
    // Initialize filesystem
    init_filesystem();
    
    // Start command shell
    start_shell();
}

// Basic video output
#define VIDEO_MEMORY 0xB8000
#define VGA_WIDTH 80
#define VGA_HEIGHT 25

static uint16_t* video_memory = (uint16_t*)VIDEO_MEMORY;
static int cursor_x = 0;
static int cursor_y = 0;

void init_video(void) {
    // Clear screen
    for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) {
        video_memory[i] = (uint16_t)' ' | (uint16_t)0x07 << 8;
    }
    cursor_x = cursor_y = 0;
}

void kprintf(const char* format, ...) {
    // Basic implementation - expand as needed
    char* str = (char*)format;
    while (*str) {
        if (*str == '\n') {
            cursor_x = 0;
            cursor_y++;
        } else {
            video_memory[cursor_y * VGA_WIDTH + cursor_x] = (uint16_t)*str | (uint16_t)0x07 << 8;
            cursor_x++;
        }
        if (cursor_x >= VGA_WIDTH) {
            cursor_x = 0;
            cursor_y++;
        }
        if (cursor_y >= VGA_HEIGHT) {
            // Scroll screen
            for (int i = 0; i < VGA_WIDTH * (VGA_HEIGHT - 1); i++) {
                video_memory[i] = video_memory[i + VGA_WIDTH];
            }
            cursor_y--;
        }
        str++;
    }
}

void init_filesystem() {
    // TODO: Implement basic filesystem support
}

void start_shell() {
    // TODO: Initialize and start command shell
} 