#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <stdint.h>

// File system error codes
#define FS_SUCCESS          0
#define FS_ERROR_NOT_FOUND  -1
#define FS_ERROR_IO        -2
#define FS_ERROR_INVALID   -3
#define FS_ERROR_FULL      -4
#define FS_ERROR_READONLY  -5

// File attributes
#define FILE_ATTR_READ_ONLY 0x01
#define FILE_ATTR_HIDDEN    0x02
#define FILE_ATTR_SYSTEM    0x04
#define FILE_ATTR_VOLUME_ID 0x08
#define FILE_ATTR_DIRECTORY 0x10
#define FILE_ATTR_ARCHIVE   0x20

typedef struct {
    char name[256];     // Full filename
    uint32_t size;      // File size in bytes
    uint8_t attributes; // File attributes
    uint16_t date;      // Last modification date
    uint16_t time;      // Last modification time
} file_entry_t;

// Filesystem operations
int init_filesystem(void);
int fs_read_file(const char* path, void* buffer, uint32_t size);
int fs_write_file(const char* path, const void* buffer, uint32_t size);
int fs_delete_file(const char* path);
int fs_get_entry(const char* path, file_entry_t* entry);

// Directory operations
int fs_list_directory(const char* path, file_entry_t* entries, uint32_t max_entries);
int fs_create_directory(const char* path);
int fs_remove_directory(const char* path);

#endif // FILESYSTEM_H 