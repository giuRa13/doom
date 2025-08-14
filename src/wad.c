#include "wad.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// WAD file always starts with a 12-byte header that contains 3 values:
// pos    lenght    name               desc
// 0x00     4     identification    the ASCII characters "IWAD" or "PWAD"
// 0x04     4     numlumps          an Integer specifying the number of lumps in the WAD
// 0x08     4     infotableofs      an Integer holding a pointer to the location of the directory

#define READ_I32(buffer, offset) \
    ((buffer)[(offset)] | ((buffer)[(offset + 1)] << 8) | ((buffer)[(offset + 2)] << 16) | ((buffer)[(offset + 3)] << 24))

int wad_load_from_file(const char* filename, wad_t* wad)
{
    if (wad == NULL) {
        return 1;
    }

    FILE* fp = fopen(filename, "rb");
    if (fp == NULL) {
        return 2;
    }

    fseek(fp, 0, SEEK_END);
    size_t size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    uint8_t* buffer = malloc(size);
    fread(buffer, size, 1, fp);
    fclose(fp);

    // read Header
    if (size < 12) {
        return 3;
    }
    wad->id = malloc(5);
    memcpy(wad->id, buffer, 4);
    wad->id[4] = 0; // null terminator
    wad->num_lumps = READ_I32(buffer, 4);
    wad->directory_offset = READ_I32(buffer, 8);

    wad->lumps = malloc(sizeof(lump_t) * wad->num_lumps);
    for (int i = 0; i < wad->num_lumps; i++) {
        uint32_t offset = wad->directory_offset + i * 16;

        wad->lumps[i].offset = READ_I32(buffer, offset);
        wad->lumps[i].size = READ_I32(buffer, offset + 4);
        wad->lumps[i].name = malloc(9);
        memcpy(wad->lumps[i].name, &buffer[offset + 8], 8);
        wad->lumps[i].name[8] = 0; // null terminator

        wad->lumps[i].data = malloc(wad->lumps[i].size);
        memcpy(wad->lumps[i].data, &buffer[wad->lumps[i].offset],
            wad->lumps[i].size);
    }

    free(buffer);
    return 0;
}

void wad_free(wad_t* wad)
{
    if (wad == NULL) {
        return;
    }

    for (int i = 0; i < wad->num_lumps; i++) {
        free(wad->lumps[i].data);
    }

    free(wad->id);
    free(wad->lumps);
    wad->num_lumps = 0;
}
