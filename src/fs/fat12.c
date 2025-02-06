#include "fs/fat12.h"
#include <string.h>

static disk_t* current_disk = NULL;
static fat12_boot_t boot_sector;
static uint8_t* fat_table = NULL;
static uint32_t data_start_sector;

// FAT12 constants
#define FAT12_EOF_CLUSTER    0xFF8
#define FAT12_BAD_CLUSTER    0xFF7
#define FAT12_DELETED_MARKER 0xE5

// Initialize FAT12 filesystem
int fat12_init(disk_t* disk) {
    current_disk = disk;
    
    // Read boot sector
    if (disk_read_sector(disk, 0, &boot_sector) != 0) {
        return -1;
    }

    // Validate filesystem
    if (boot_sector.bytes_per_sector != 512 ||
        boot_sector.number_of_fats == 0) {
        return -1;
    }

    // Calculate important offsets
    uint32_t fat_start = boot_sector.reserved_sectors;
    uint32_t root_dir_start = fat_start + 
        (boot_sector.number_of_fats * boot_sector.sectors_per_fat);
    data_start_sector = root_dir_start + 
        ((boot_sector.root_dir_entries * 32) / boot_sector.bytes_per_sector);

    // Allocate and read FAT table
    fat_table = (uint8_t*)malloc(boot_sector.sectors_per_fat * boot_sector.bytes_per_sector);
    if (!fat_table) {
        return -1;
    }

    // Read first FAT table
    for (int i = 0; i < boot_sector.sectors_per_fat; i++) {
        if (disk_read_sector(disk, fat_start + i, 
            fat_table + (i * boot_sector.bytes_per_sector)) != 0) {
            free(fat_table);
            return -1;
        }
    }

    return 0;
}

// Helper function to get next cluster in chain
static uint16_t get_next_cluster(uint16_t current_cluster) {
    uint32_t fat_offset = current_cluster + (current_cluster / 2);
    uint16_t fat_value = *(uint16_t*)(fat_table + fat_offset);
    
    if (current_cluster & 1) {
        fat_value >>= 4;    // Odd cluster number
    } else {
        fat_value &= 0x0FFF; // Even cluster number
    }
    
    return fat_value;
}

// Helper function to read a cluster
static int read_cluster(uint16_t cluster, void* buffer) {
    uint32_t sector = data_start_sector + (cluster - 2) * boot_sector.sectors_per_cluster;
    
    for (int i = 0; i < boot_sector.sectors_per_cluster; i++) {
        if (disk_read_sector(current_disk, sector + i, 
            (uint8_t*)buffer + (i * boot_sector.bytes_per_sector)) != 0) {
            return -1;
        }
    }
    
    return 0;
}

// Helper function to find file in root directory
static int find_file(const char* filename, fat12_dir_entry_t* entry) {
    uint32_t root_dir_sectors = ((boot_sector.root_dir_entries * 32) + 
        (boot_sector.bytes_per_sector - 1)) / boot_sector.bytes_per_sector;
    uint32_t root_dir_start = boot_sector.reserved_sectors + 
        (boot_sector.number_of_fats * boot_sector.sectors_per_fat);
    
    // Allocate buffer for root directory
    fat12_dir_entry_t* dir_buffer = (fat12_dir_entry_t*)malloc(boot_sector.bytes_per_sector);
    if (!dir_buffer) return -1;
    
    // Convert filename to 8.3 format
    char file_8_3[12];
    memset(file_8_3, ' ', 11);
    file_8_3[11] = '\0';
    
    const char* ext = strrchr(filename, '.');
    if (ext) {
        size_t name_len = ext - filename;
        if (name_len > 8) name_len = 8;
        memcpy(file_8_3, filename, name_len);
        
        ext++; // Skip the dot
        size_t ext_len = strlen(ext);
        if (ext_len > 3) ext_len = 3;
        memcpy(file_8_3 + 8, ext, ext_len);
    } else {
        size_t name_len = strlen(filename);
        if (name_len > 8) name_len = 8;
        memcpy(file_8_3, filename, name_len);
    }
    
    // Search through root directory
    for (uint32_t sector = 0; sector < root_dir_sectors; sector++) {
        if (disk_read_sector(current_disk, root_dir_start + sector, dir_buffer) != 0) {
            free(dir_buffer);
            return -1;
        }
        
        // Check each entry in the sector
        for (uint32_t i = 0; i < boot_sector.bytes_per_sector / sizeof(fat12_dir_entry_t); i++) {
            if (dir_buffer[i].filename[0] == 0) {
                // End of directory
                free(dir_buffer);
                return -1;
            }
            
            if (dir_buffer[i].filename[0] == FAT12_DELETED_MARKER) {
                continue;
            }
            
            // Compare filename
            if (memcmp(dir_buffer[i].filename, file_8_3, 11) == 0) {
                memcpy(entry, &dir_buffer[i], sizeof(fat12_dir_entry_t));
                free(dir_buffer);
                return 0;
            }
        }
    }
    
    free(dir_buffer);
    return -1;
}

// Now implement the file reading function
int fat12_read_file(const char* filename, void* buffer, uint32_t size) {
    fat12_dir_entry_t entry;
    if (find_file(filename, &entry) != 0) {
        return -1; // File not found
    }
    
    // Check if size is sufficient
    if (size < entry.file_size) {
        return -1; // Buffer too small
    }
    
    // Read file clusters
    uint16_t current_cluster = entry.starting_cluster;
    uint32_t bytes_read = 0;
    uint8_t* buf_ptr = (uint8_t*)buffer;
    
    while (current_cluster < FAT12_EOF_CLUSTER && bytes_read < entry.file_size) {
        // Read current cluster
        if (read_cluster(current_cluster, buf_ptr) != 0) {
            return -1;
        }
        
        // Update pointers and counters
        uint32_t cluster_size = boot_sector.bytes_per_sector * boot_sector.sectors_per_cluster;
        buf_ptr += cluster_size;
        bytes_read += cluster_size;
        
        // Get next cluster
        current_cluster = get_next_cluster(current_cluster);
        if (current_cluster == FAT12_BAD_CLUSTER) {
            return -1;
        }
    }
    
    return bytes_read;
}

// Write a file to the filesystem
int fat12_write_file(const char* filename, const void* buffer, uint32_t size) {
    // TODO: Implement file writing
    // 1. Find/create directory entry
    // 2. Allocate clusters
    // 3. Write data
    // 4. Update FAT
    return -1;
}

// Delete a file from the filesystem
int fat12_delete_file(const char* filename) {
    // TODO: Implement file deletion
    // 1. Find file in directory
    // 2. Mark directory entry as deleted
    // 3. Free FAT chain
    return -1;
}

// List directory contents
int fat12_list_directory(const char* path) {
    // TODO: Implement directory listing
    // 1. Find directory in root
    // 2. List all valid entries
    return -1;
} 