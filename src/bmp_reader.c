#include <ctoy.h> // ctoy API (including frequently used ANSI C libs)

typedef struct {
    uint8_t *ptr;
    size_t size;
} MEMPTR;

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
    MEMPTR mem = readFileToMemory(fname);

    if (!mem.ptr) {
        return;
    }

    printf("File size: %zu bytes\n", mem.size);
    uint8_t firstByte = readBYTE(mem.ptr, 0, mem.size);
    printf("First byte: 0x%02X\n", firstByte);

    free(mem.ptr);
}

void ctoy_main_loop() // called at every update of the main loop
{}

void ctoy_end() // called at the end of the program
{}
