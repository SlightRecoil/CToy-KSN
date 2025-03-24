#include <ctoy.h> // ctoy API (including frequently used ANSI C libs)

typedef struct {
    uint8_t *ptr;
    size_t size;
} MEMPTR;

// Plan for functions: readFileToMemory(const char * filename) and uint8_t readBYTE( uint8_t *data, size_t offset)
MEMPTR readFileToMemory(const char *filename) {
    MEMPTR mem = {NULL, 0};
    FILE *fp = fopen(filename, "rb");
    if (!fp) {
        perror("File opening failed");
        return mem;
    }

    if (fseek(fp, 0L, SEEK_END) != 0) {
        perror("Seeking file failed");
        fclose(fp);
        return mem;
    }
    mem.size = ftell(fp);
    rewind(fp);

    mem.ptr = malloc(mem.size);
    if (!mem.ptr) {
        perror("Memory allocation failed");
        fclose(fp);
        return mem;
    }

    if (fread(mem.ptr, sizeof(uint8_t), mem.size, fp) != mem.size) {
        perror("File reading failed");
        free(mem.ptr);
        mem.ptr = NULL;
        mem.size = 0;
    }

    fclose(fp);
    return mem;
}

uint8_t readBYTE(uint8_t *data, size_t offset, size_t size) {
    if (offset >= size) {
        fprintf(stderr, "readBYTE: Offset out of bounds\n");
        return 0;
    }
    return data[offset];
}

void ctoy_begin() {
    printf("Hello, World!\n");
    const char* fname = "data/tetris.bmp";

    // aufgabe: open and read the file into memory
    MEMPTR mem = readFileToMemory(fname);

    // aufgabe: handle errors from malloc and fread
    if (!mem.ptr) {
        return;
    }

    printf("File size: %zu bytes\n", mem.size);
    uint8_t firstByte = readBYTE(mem.ptr, 0, mem.size);
    printf("First byte: 0x%02X\n", firstByte);

    free(mem.ptr);
}

    //✅ fehlerbehandlung malloc, fread
    //✅ in einer func readFileToMemory( const char *filename)
    //✅ returnwert: struct (ptr, size) ; typedef MEMPTR
    //✅ unit8_t readBYTE(unit_8 *data, size_t offset)
    //unit16_t readword(unit_8 *data, size_t offset)
    //uint32 readdoubleworld(unit_8 *data, size_t offset)
    //int32 readlong(unit_8 *data, size_t offset)
    // IMPORTANT: NOTE WHICH NEED LITTLE AND BIG ENDIAN
/* H Ü

assignHead() // Weißt BAR zag den richtigen speicher orten zu. Danach soll zag.bfSize gehen
printHead() // Prints the full 14 Byte header

*/


void ctoy_main_loop() // called at every update of the main loop
{}

void ctoy_end() // called at the end of the program
{}
