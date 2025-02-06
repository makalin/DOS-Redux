#ifndef FAT12_H
#define FAT12_H

#include "fs/filesystem.h"
#include "fs/disk.h"

// FAT12 specific structures
typedef struct {
    uint8_t jump[3];
    char oem_name[8];
    uint16_t bytes_per_sector;
    uint8_t sectors_per_cluster;
    uint16_t reserved_sectors;
    uint8_t number_of_fats;
    uint16_t root_dir_entries;
    uint16_t total_sectors;
    uint8_t media_descriptor;
    uint16_t sectors_per_fat;
    uint16_t sectors_per_track;
    uint16_t number_of_heads;
    uint32_t hidden_sectors;
    uint32_t large_sector_count;
} __attribute__((packed)) fat12_boot_t;

typedef struct {
    char filename[8];
    char extension[3];
    uint8_t attributes;
    uint8_t reserved[10];
    uint16_t time;
    uint16_t date;
    uint16_t starting_cluster;
    uint32_t file_size;
} __attribute__((packed)) fat12_dir_entry_t;

// FAT12 filesystem operations
int fat12_init(disk_t* disk);
int fat12_read_file(const char* filename, void* buffer, uint32_t size);
int fat12_write_file(const char* filename, const void* buffer, uint32_t size);
int fat12_delete_file(const char* filename);
int fat12_list_directory(const char* path);

#endif // FAT12_H 