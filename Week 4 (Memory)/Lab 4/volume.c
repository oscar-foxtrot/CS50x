// Modifies the volume of an audio file

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define bufsize 512

// Number of bytes in .wav header
const int HEADER_SIZE = 44;


int main(int argc, char *argv[])
{
    // Check command-line arguments
    if (argc != 4)
    {
        printf("Usage: ./volume input.wav output.wav factor\n");
        return 1;
    }

    // Open files and determine scaling factor
    FILE *input = fopen(argv[1], "r");
    if (input == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    FILE *output = fopen(argv[2], "w");
    if (output == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    float factor = atof(argv[3]);

    // Copy header from input file to output file
    char *buf = malloc(HEADER_SIZE);
    fread(buf, HEADER_SIZE, 1, input);
    fwrite(buf, HEADER_SIZE, 1, output);
    free(buf);

    // Read samples from input file and write updated data to output file
    buf = malloc(bufsize);
    // It may be more time-effective to read by chunks
    int count;
    do
    {
        count = fread(buf, 1, bufsize, input);
        // Modify data
        for (int i = 0; i < count; i += 2)
        {
            int16_t tmp;
            memcpy(&tmp, buf + i, 2);
            tmp *= factor;
            memcpy(buf + i, &tmp, 2);
        }

        fwrite(buf, 1, count, output);
    }
    while (count == bufsize);
    free(buf);
    // Close files
    fclose(input);
    fclose(output);
}
