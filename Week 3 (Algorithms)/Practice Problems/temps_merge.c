// Practice working with structs
// Practice applying sorting algorithms

#include <cs50.h>
#include <stdio.h>

#define NUM_CITIES 10

typedef struct
{
    string city;
    int temp;
}
avg_temp;

avg_temp temps[NUM_CITIES];

void sort_cities(int beg, int end);

int main(void)
{
    temps[0].city = "Austin";
    temps[0].temp = 97;

    temps[1].city = "Boston";
    temps[1].temp = 82;

    temps[2].city = "Chicago";
    temps[2].temp = 85;

    temps[3].city = "Denver";
    temps[3].temp = 90;

    temps[4].city = "Las Vegas";
    temps[4].temp = 105;

    temps[5].city = "Los Angeles";
    temps[5].temp = 82;

    temps[6].city = "Miami";
    temps[6].temp = 97;

    temps[7].city = "New York";
    temps[7].temp = 85;

    temps[8].city = "Phoenix";
    temps[8].temp = 107;

    temps[9].city = "San Francisco";
    temps[9].temp = 66;

    sort_cities(0, NUM_CITIES - 1);

    printf("\nAverage July Temperatures by City\n\n");

    for (int i = 0; i < NUM_CITIES; i++)
    {
        printf("%s: %i\n", temps[i].city, temps[i].temp);
    }
}

avg_temp tmparr[NUM_CITIES];

void sort_cities(int beg, int end)
{
    if (beg == end)
    {
        return;
    }
    int fhend = (end + beg) / 2; //first half's last elem (number)

    sort_cities(beg, fhend);
    sort_cities(fhend + 1, end);

    avg_temp tmp[end - beg + 1];
    int cur1 = beg, cur2 = fhend + 1, cntr = 0;

    while (cur1 <= fhend && cur2 <= end)
    {
        if (temps[cur1].temp < temps[cur2].temp)
        {
            tmp[cntr].temp = temps[cur2].temp;
            tmp[cntr].city = temps[cur2].city;
            ++cur2;
        }
        else
        {
            tmp[cntr].temp = temps[cur1].temp;
            tmp[cntr].city = temps[cur1].city;
            ++cur1;
        }
        ++cntr;
    }

    // Putting remaining elems into the temporary array
    // Note: Watch "Merge sort" from the SHORTS by CS50x, Week 3, for reference

    if (cur2 > end)
    {
        while (cur1 <= fhend)
        {
            tmp[cntr].temp = temps[cur1].temp;
            tmp[cntr].city = temps[cur1].city;
            ++cur1;
            ++cntr;
        }
    }

    for (int i = 0; i < cntr; ++i)
    {
        temps[i + beg].temp = tmp[i].temp;
        temps[i + beg].city = tmp[i].city;
    }
}
