#include <ctoy.h> // ctoy API (including frequently used ANSI C libs)

/*
 * bmp_reader.c
 * 
 * This file contains functions for reading BMP files into memory,
 * accessing their headers, and interpreting their data. It includes
 * functionality to read bytes, words, and double words from the
 * BMP file format.
 */

typedef struct {
    uint8_t *ptr; // Pointer to the memory where file data is stored
    size_t size;  // Size of the allocated memory
} MEMPTR;

// Reads the contents of a file into memory.
// Parameters:
// - filename: The name of the file to read.
// Returns: A MEMPTR structure containing a pointer to the file data
//          and its size.
MEMPTR readFileToMemory(const char *filename) {
    MEMPTR mem = {NULL, 0}; // Initialize memory pointer and size
    FILE *fp = fopen(filename, "rb"); // Open the file in binary read mode
    if (!fp) {
        perror("File opening failed"); // Error handling for file opening
        return mem;
    }

    if (fseek(fp, 0L, SEEK_END) != 0) { // Seek to the end of the file
        perror("Seeking file failed"); // Error handling for seeking
        fclose(fp);
        return mem;
    }
    mem.size = ftell(fp); // Get the size of the file
    rewind(fp); // Rewind the file pointer to the beginning

    mem.ptr = malloc(mem.size); // Allocate memory for file data
    if (!mem.ptr) {
        perror("Memory allocation failed"); // Error handling for memory allocation
        fclose(fp);
        return mem;
    }

    if (fread(mem.ptr, sizeof(uint8_t), mem.size, fp) != mem.size) { // Read file data
        perror("File reading failed"); // Error handling for reading
        free(mem.ptr); // Free allocated memory on failure
        mem.ptr = NULL;
        mem.size = 0;
    }

    fclose(fp); // Close the file
    return mem; // Return the memory pointer and size
}

// Reads a single byte from the data at the specified offset.
// Parameters:
// - data: Pointer to the data buffer.
// - offset: The position in the data buffer to read from.
// - size: The total size of the data buffer.
// Returns: The byte read from the specified offset or 0 if out of bounds.
uint8_t readBYTE(uint8_t *data, size_t offset, size_t size) {
    if (offset >= size) { // Check for out of bounds
        fprintf(stderr, "readBYTE: Offset out of bounds\n");
        return 0; // Return 0 if out of bounds
    }
    return data[offset]; // Return the byte at the specified offset
}

// Reads a 16-bit word from the data at the specified offset.
// Parameters:
// - data: Pointer to the data buffer.
// - offset: The position in the data buffer to read from.
// - size: The total size of the data buffer.
// Returns: The 16-bit word read from the specified offset or 0 if out of bounds.
uint16_t readWORD(uint8_t *data, size_t offset, size_t size) {
    if (offset + 1 >= size) { // Check for out of bounds
        fprintf(stderr, "readWORD: Offset out of bounds\n");
        return 0; // Return 0 if out of bounds
    }
    return data[offset] | (data[offset + 1] << 8); // Combine bytes to form a word
}

// Reads a 32-bit double word from the data at the specified offset.
// Parameters:
// - data: Pointer to the data buffer.
// - offset: The position in the data buffer to read from.
// - size: The total size of the data buffer.
// Returns: The 32-bit double word read from the specified offset or 0 if out of bounds.
uint32_t readDWORD(uint8_t *data, size_t offset, size_t size) {
    if (offset + 3 >= size) { // Check for out of bounds
        fprintf(stderr, "readDWORD: Offset out of bounds\n");
        return 0; // Return 0 if out of bounds
    }
    return data[offset] | (data[offset + 1] << 8) | (data[offset + 2] << 16) | (data[offset + 3] << 24); // Combine bytes to form a double word
}

// Reads a 32-bit signed long from the data at the specified offset.
// Parameters:
// - data: Pointer to the data buffer.
// - offset: The position in the data buffer to read from.
// - size: The total size of the data buffer.
// Returns: The 32-bit signed long read from the specified offset or 0 if out of bounds.
int32_t readLONG(uint8_t *data, size_t offset, size_t size) {
    if (offset + 3 >= size) { // Check for out of bounds
        fprintf(stderr, "readLONG: Offset out of bounds\n");
        return 0; // Return 0 if out of bounds
    }
    return (int32_t)(data[offset] | (data[offset + 1] << 8) | (data[offset + 2] << 16) | (data[offset + 3] << 24)); // Combine bytes to form a signed long
}

void ctoy_begin() {
    printf("Hello, World!\n");
    const char* fname = "data/tetris.bmp"; // File name of the BMP to read

    // Open and read the file into memory
    MEMPTR mem = readFileToMemory(fname);

    // Handle errors from malloc and fread
    if (!mem.ptr) {
        return; // Exit if memory allocation or file reading failed
    }

    printf("File size: %zu bytes\n", mem.size); // Print the size of the file
    uint8_t firstByte = readBYTE(mem.ptr, 0, mem.size); // Read the first byte of the file
    printf("First byte: 0x%02X\n", firstByte); // Print the first byte in hexadecimal format

    free(mem.ptr); // Free the allocated memory
}

// Assigns values to the members of the BITMAPFILEHEADER struct from the data buffer.
// Parameters:
// - zag: Pointer to a BITMAPFILEHEADER struct where the values will be stored.
// - data: Pointer to the data buffer containing the BMP header.
// - size: The total size of the data buffer.
void assignHead(BITMAPFILEHEADER *zag, uint8_t *data, size_t size) {
    if (size < 14) { // Check if the data size is sufficient
        fprintf(stderr, "assignHead: Insufficient data size\n");
        return; // Exit if insufficient data
    }
    zag->bfType = readWORD(data, 0, size); // Access and assign bfType from the BMP header
    zag->bfSize = readDWORD(data, 2, size); // Access and assign bfSize from the BMP header
    zag->bfReserved1 = readWORD(data, 6, size); // Access and assign bfReserved1 from the BMP header
    zag->bfReserved2 = readWORD(data, 8, size); // Access and assign bfReserved2 from the BMP header
    zag->bfOffBits = readDWORD(data, 10, size); // Access and assign bfOffBits from the BMP header
}

// Prints the full 14-byte header of the BITMAPFILEHEADER struct.
// Parameters:
// - zag: Pointer to a BITMAPFILEHEADER struct containing the header information.
void printHead(const BITMAPFILEHEADER *zag) {
    printf("BITMAPFILEHEADER:\n");
    printf("  bfType: 0x%X\n", zag->bfType); // Print the bfType
    printf("  bfSize: %u bytes\n", zag->bfSize); // Print the bfSize
    printf("  bfReserved1: 0x%X\n", zag->bfReserved1); // Print the bfReserved1
    printf("  bfReserved2: 0x%X\n", zag->bfReserved2); // Print the bfReserved2
    printf("  bfOffBits: %u\n", zag->bfOffBits); // Print the bfOffBits
}

void ctoy_main_loop() // Called at every update of the main loop
{}

void ctoy_end() // Called at the end of the program
{}
