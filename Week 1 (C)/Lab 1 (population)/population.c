#include <cs50.h>
#include <stdio.h>

int main(void)
{
    // Input: startsize
    int stsiz;
    do
    {
        stsiz = get_int("Start size: ");
    }
    while (stsiz < 9);

    // Input: endsize
    int endsiz;
    do
    {
        endsiz = get_int("End size: ");
    }
    while (endsiz < stsiz);

    // Calculating the number of years
    int yrs = 0, cursiz = stsiz;
    while (cursiz < endsiz)
    {
        cursiz += cursiz / 3 - cursiz / 4;
        ++yrs;
    }
    printf("Years: %d\n", yrs);

    return 0;
}
