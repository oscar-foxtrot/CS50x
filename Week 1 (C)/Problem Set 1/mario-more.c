#include <cs50.h>
#include <stdio.h>

int main(void)
{
    // Ask the user for input: Pyramid height
    int height;
    do
    {
        height = get_int("Height: ");
    }
    while (height < 1 || height > 8);

    // Printing the pyramid
    for (int curline = height; curline > 0; --curline)
    {
        // On the current line:
        // 1st half
        for (int j = 1; j < curline; ++j)
        {
            printf(" ");
        }
        for (int j = height - curline + 1; j > 0; --j)
        {
            printf("#");
        }
        // the middle
        printf("  ");
        // 2nd half
        for (int j = height - curline + 1; j > 0; --j)
        {
            printf("#");
        }
        printf("\n");
    }

    return 0;
}
