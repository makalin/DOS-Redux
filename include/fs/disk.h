#ifndef DISK_H
#define DISK_H

#include <stdint.h>

// Disk I/O interface
typedef struct {
    uint8_t drive_number;
    uint16_t bytes_per_sector;
    uint8_t sectors_per_cluster;
    uint16_t reserved_sectors;
    uint32_t total_sectors;
} disk_t;

// Disk operations
int disk_init(disk_t* disk, uint8_t drive_number);
int disk_read_sector(disk_t* disk, uint32_t sector, void* buffer);
int disk_write_sector(disk_t* disk, uint32_t sector, const void* buffer);
int disk_get_params(disk_t* disk);

#endif // DISK_H 