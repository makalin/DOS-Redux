#include "fs/filesystem.h"
#include "fs/fat12.h"
#include "fs/disk.h"

static disk_t system_disk;
static int fs_initialized = 0;

int init_filesystem(void) {
    // Initialize disk
    if (disk_init(&system_disk, 0x00) != 0) {
        return -1;
    }

    // Initialize FAT12 filesystem
    if (fat12_init(&system_disk) != 0) {
        return -1;
    }

    fs_initialized = 1;
    return 0;
}

// File operations wrapper functions
int fs_read_file(const char* path, void* buffer, uint32_t size) {
    if (!fs_initialized) return -1;
    return fat12_read_file(path, buffer, size);
}

int fs_write_file(const char* path, const void* buffer, uint32_t size) {
    if (!fs_initialized) return -1;
    return fat12_write_file(path, buffer, size);
}

int fs_delete_file(const char* path) {
    if (!fs_initialized) return -1;
    return fat12_delete_file(path);
}

int fs_get_entry(const char* path, file_entry_t* entry) {
    // TODO: Implement get entry wrapper
    return -1;
} 