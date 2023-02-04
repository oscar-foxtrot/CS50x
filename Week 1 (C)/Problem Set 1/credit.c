#include <cs50.h>
#include <stdio.h>

// Returns the issuer's company name (or INVALID if not valid)
string get_issuer(long card_number);

int main(void)
{
    long cardnum = get_long("Number: ");
    printf("%s\n", get_issuer(cardnum));

    return 0;
}

// Checks the validity of the card number (Luhn's algo)
int check_validity(long card_number);

string get_issuer(long num)
{
    if (!check_validity(num))
    {
        return "INVALID";
    }
    else
    {
        // len = length of the card number (digits)
        int len = 2;
        while (num > 99)
        {
            ++len;
            num /= 10;
        }

        int first = num / 10; //first digit of card number
        int second = num % 10; //second digit of card number
        switch (first)
        {
            case 5:
                if (second >= 1 && second <= 5 && len == 16)
                {
                    return "MASTERCARD";
                }
                else
                {
                    return "INVALID";
                }
            case 4:
                if (len == 13 || len == 16)
                {
                    return "VISA";
                }
                else
                {
                    return "INVALID";
                }
            case 3:
                if ((second == 4 || second == 7) && len == 15)
                {
                    return "AMEX";
                }
                else
                {
                    return "INVALID";
                }
            default:
                return "INVALID";
        }
    }
}


int check_validity(long num)
{
    // Luhn's algo
    int sum = 0;
    while (num > 0)
    {
        sum += num % 10;
        int tmp = (num / 10) % 10 * 2;
        sum += tmp / 10 + tmp % 10;
        num /= 100;
    }
    return (sum % 10) == 0 ? 1 : 0;
}
