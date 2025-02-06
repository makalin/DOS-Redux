#include "fs/disk.h"

// BIOS interrupt for disk operations
#define BIOS_DISK_INTERRUPT 0x13

// Initialize disk
int disk_init(disk_t* disk, uint8_t drive_number) {
    disk->drive_number = drive_number;
    return disk_get_params(disk);
}

// Read a sector from disk using BIOS
int disk_read_sector(disk_t* disk, uint32_t sector, void* buffer) {
    // Convert LBA to CHS
    uint16_t cylinder = sector / (16 * 63);
    uint8_t head = (sector / 63) % 16;
    uint8_t sector_number = (sector % 63) + 1;

    __asm__ volatile (
        "int $0x13"
        : // No output
        : "a"(0x0201),      // AH=02 (read), AL=1 (sectors)
          "d"((head << 8) | disk->drive_number),
          "c"((cylinder << 8) | sector_number),
          "b"(buffer)
        : "memory"
    );

    return 0; // TODO: Check for errors
}

// Write a sector to disk using BIOS
int disk_write_sector(disk_t* disk, uint32_t sector, const void* buffer) {
    // Similar to read_sector but with AH=03
    // TODO: Implement write operation
    return -1;
}

// Get disk parameters using BIOS
int disk_get_params(disk_t* disk) {
    // TODO: Implement getting disk parameters
    disk->bytes_per_sector = 512;  // Standard for floppy/hard disks
    disk->sectors_per_cluster = 1;
    disk->reserved_sectors = 1;
    disk->total_sectors = 2880;    // Standard 1.44MB floppy
    return 0;
} 