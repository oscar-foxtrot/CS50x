#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "wav.h"

int check_format(WAVHEADER header);
int get_block_size(WAVHEADER header);

int main(int argc, char *argv[])
{
    // Ensure proper usage
    if (argc != 3)
    {
        printf("Usage: ./reverse input.wav output.wav\n");
        return 1;
    }

    // Open input file for reading
    FILE *inptr;
    if (!(inptr = fopen(argv[1], "r")))
    {
        return 1;
    }

    // Read header into an array
    WAVHEADER hdr;
    fread(&hdr, 1, sizeof(WAVHEADER), inptr);

    // Use check_format to ensure WAV format
    if (!check_format(hdr))
    {
        printf("Input is not a WAV file.\n");
        return 1;
    }

    // Open output file for writing
    FILE *outptr;
    if (!(outptr = fopen(argv[2], "w")))
    {
        return 1;
    }

    // Write header to file
    fwrite(&hdr, 1, sizeof(WAVHEADER), outptr);

    // Use get_block_size to calculate size of block
    int blocksize = get_block_size(hdr);

    // Write reversed audio to file
    if (fseek(inptr, -blocksize, SEEK_END))
    {
        printf("File is empty or an error has occured while reading the file\n");
        return 1;
    }
    void *sample = malloc(blocksize);

    while (ftell(inptr) >= sizeof(WAVHEADER))
    {
        fread(sample, 1, blocksize, inptr);
        fwrite(sample, 1, blocksize, outptr);
        fseek(inptr, -2 * blocksize, SEEK_CUR);
    }

    free(sample);
    fclose(inptr);
    fclose(outptr);
    return 0;
}

int check_format(WAVHEADER header)
{
    for (int i = 0; i < 4; ++i)
    {
        if (header.format[i] != "WAVE"[i])
        {
            return 0;
        }
    }
    return 1;
}

int get_block_size(WAVHEADER header)
{
    return (int)header.bitsPerSample * header.numChannels / 8;
}
