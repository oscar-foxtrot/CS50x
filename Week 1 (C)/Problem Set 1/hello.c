#include <stdio.h>
#include <cs50.h>

//Asks for name and outputs one line

int main(void)
{
    string name = get_string("What's your name? ");
    printf("hello, %s\n", name);
}
