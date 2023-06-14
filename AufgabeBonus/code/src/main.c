#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <malloc.h>
#include <errno.h>
#include <sys/stat.h>
#include "../include/rle.h"


// Forward declarations
off_t get_file_size(int fd);
char* get_compressed_file_path(const char *filePath);
char* get_decompressed_file_path(const char *filePath);

// Type definitions
typedef void (*FirstOperation)(RLE* rle, const char* data, size_t size);
typedef char* (*SecondOperation)(RLE* rle, size_t* size);
typedef char* (*FilePathFunction)(const char *);

// The operation enum
typedef enum {
    COMPRESS,
    DECOMPRESS,
    OPERATION_COUNT // always keep this as the last element to get the count of operations
} Operation;

// These operations are used to fill up the RLE data structure
FirstOperation PrepareOutputOperation[OPERATION_COUNT] = {
        encode_rle,
        deserialize_rle
};

// These operations are used to create the final output data
SecondOperation FinalizeOutputOperation[OPERATION_COUNT] = {
        serialize_rle,
        decode_rle
};

// These operations are used to create the final output data
FilePathFunction OutputFilePath[OPERATION_COUNT] = {
        get_compressed_file_path,
        get_decompressed_file_path
};

int main (int argc, char *argv[] ) {
    if (argc < 2 || argc > 3) {
        printf("Usage: %s <filepath> [operation]\n", argv[0]);
        printf("operation: '-d' for decompress, '-c' for compression (default)\n");
        return 1;
    }
    Operation op = (argc == 3) && (strcmp(argv[2], "-d") == 0) ? DECOMPRESS : COMPRESS;

    char* path = argv[1];
    int fd = open(path, O_RDONLY);
    if (fd == -1) {
        printf("Error: could not open file %s\n", path);
        printf("Error number: %d\n", errno);
        perror("Error message");
        return 1;
    }

    off_t size = get_file_size(fd);
    if (size == -1) {
        printf("Error: could not get file size\n");
        printf("Error number: %d\n", errno);
        perror("Error message");
        return 1;
    }

    char* buffer = malloc(size);
    size_t bytes_to_read = size;
    ssize_t bytes_read;
    if ((bytes_read = read(fd, buffer, sizeof(buffer))) != bytes_to_read) {
        printf("Error: read file %s incomplete\n", path);
        if (bytes_read == -1) {
            printf("Error number: %d\n", errno);
            perror("Error message");
        }
        return 1;
    }
    close(fd);

    RLE* rle = create_rle();
    PrepareOutputOperation[op](rle, buffer, bytes_read);;
    free(buffer);

    printf("RLE counts:\n");
    print_rle(rle, 1);

    char* outPath = OutputFilePath[op](path);
    fd = open(outPath, O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (fd == -1) {
        printf("Error: could not open output file %s\n", outPath);
        printf("Error number: %d\n", errno);
        perror("Error message");
        return 1;
    }

    size_t bytes_to_write = 0;
    char* data = FinalizeOutputOperation[op](rle, &bytes_to_write);
    ssize_t bytes_written;
    if ((bytes_written = write(fd, data, bytes_to_write)) < ((ssize_t) bytes_to_write)) {
        printf("Error: write to file %s failed\n", path);
        if (bytes_written == -1) {
            printf("Error number: %d\n", errno);
            perror("Error message");
        }
        return 1;
    }
    close(fd);
    free(data);
    delete_rle(rle);

    printf("Done.\n");

    return 0;
}

off_t get_file_size(int fd) {
    struct stat buf;
    return (fstat(fd, &buf) < 0) ? -1 : buf.st_size;
}

char* get_compressed_file_path(const char *filePath) {
    char *dot = strrchr(filePath, '.'); // find last '.'
    size_t len = dot ? (size_t) (dot - filePath) : strlen(filePath); // if no '.', use whole string
    char *newPath = (char*) malloc(len + 5); // 5 for ".mrl" and '\0'
    strncpy(newPath, filePath, len); // copy the part before '.'
    strcpy(newPath + len, ".mrl"); // append ".mrl"
    return newPath;
}

char* get_decompressed_file_path(const char *filePath) {
    char *dot = strrchr(filePath, '.'); // find last '.'
    if (dot && strcmp(dot, ".mrl") == 0) { // if ".mrl" exists
        size_t len = dot - filePath; // get the length of the part before '.'
        char *newPath = (char*) malloc(len + 1); // 1 for '\0'
        strncpy(newPath, filePath, len); // copy the part before '.'
        newPath[len] = '\0'; // append '\0'
        return newPath;
    }
    return strdup(filePath); // if no ".mrl", return a copy of original filePath
}
