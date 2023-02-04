#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

// Count the number of letters in |txt|
int count_letters(string txt);

// Count the number of '.', '!' and '?' in |txt|
int count_sentences(string txt);

// Count the number of sequences of letters in |txt| (based on the special symbols: ',', '.' etc)
int count_words(string txt);

int main(void)
{
    string txt = get_string("Text: ");
    if (strlen(txt) == 0)
    {
        return 0;
    }
    int lets = count_letters(txt);
    int wrds = count_words(txt);
    int sents = count_sentences(txt);
    int index = (0.0588 * lets / wrds * 100) - (0.296 * sents / wrds * 100) - 15.8 + 0.5; //0.5 for rounding

    if (index < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (index >= 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %d\n", index);
    }

    return 0;
}

////////////////////////////////////
int count_words(string txt)
{
    int count = 0;
    for (int i = 1, len = strlen(txt); i < len; ++i)
    {
        char c1 = txt[i - 1];
        char c2 = txt[i];
        if (isalpha(c1) &&
            (c2 == ',' || c2 == '.' || c2 == ';' || c2 == ':' || c2 == '?' || c2 == '!'
             || c2 == '$' || c2 == '\"' || c2 == ')' || c2 == '(' || c2 == ' '
             || c2 == '{' || c2 == '}' || c2 == '[' || c2 == ']'))
        {
            ++count;
        }
    }
    return count;
}

int count_sentences(string txt)
{
    int count = 0;
    for (int i = 0, len = strlen(txt); i < len; ++i)
    {
        char c = txt[i];
        if (c == '.' || c == '?' || c == '!')
        {
            ++count;
        }
    }
    return count;
}

int count_letters(string txt)
{
    int count = 0;
    for (int i = 0, len = strlen(txt); i < len; ++i)
    {
        char c = txt[i];
        if ((c <= 'z' && c >= 'a') || (c <= 'Z' && c >= 'A'))
        {
            ++count;
        }
    }
    return count;
}
