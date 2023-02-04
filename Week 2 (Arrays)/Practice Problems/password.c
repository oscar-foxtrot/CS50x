#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

int validate(char *passwd);

int main(int argc, char *argv[])
{
    char *s = malloc(256);
    printf("Create password: ");
    scanf("%s", s);
    validate(s) ? printf("New password set\n") :
        printf("Your password must contain at least one uppercase letter, one lowercase letter, one printable symbol and one digit\n");

    free(s);
    return 0;
}



int validate(char *passwd)
{
    int upr = 0, lowr = 0, digt = 0, prt = 0;
    for (int i = 0, plen = strlen(passwd); i <= plen; ++i)
    {
        char c = passwd[i];
        if (isupper(c))
        {
            upr = 1;
        }
        else if (islower(c))
        {
            lowr = 1;
        }
        else if (isdigit(c))
        {
            digt = 1;
        }
        else if (isprint(c))
        {
            prt = 1;
        }
    }

    return (upr && lowr && digt && prt);
}
