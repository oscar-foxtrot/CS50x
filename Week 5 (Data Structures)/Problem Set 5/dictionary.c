// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

unsigned int words = 0;

// Number of letters in the alphabet
#define NLETTERS 26
// Choose number of buckets in hash table
const unsigned int N = NLETTERS * NLETTERS * NLETTERS * NLETTERS / 3;
const unsigned int shiftv = 4; //ceil(log(2, N))

// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    node *curnode = table[hash(word)];
    while (curnode != NULL)
    {
        char tmpc1 = word[0], tmpc2 = curnode->word[0];
        for (int i = 0; tmpc1 != '\0'; ++i)
        {
            if (tolower(tmpc1) != tolower(tmpc2))
            {
                break;
            }
            tmpc1 = word[i + 1];
            tmpc2 = curnode->word[i + 1];
        }
        if (tmpc1 == tmpc2)
        {
            return true;
        }

        curnode = curnode->next;
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    unsigned int res = 0;
    char tmpc;
    int i = 0;
    while ((tmpc = tolower(word[i++])) != '\0')
    {
        res <<= shiftv;
        res += tmpc;
    }
    return res % N;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    FILE *fdict = fopen(dictionary, "r");
    if (!fdict)
    {
        return false;
    }

    char word[LENGTH + 1];

    // Hash each word
    // Part of code borrowed from "speller.c"
    int index = 0;
    char c;
    while (fread(&c, sizeof(char), 1, fdict))
    {
        // Allow only alphabetical characters and apostrophes
        if (isalpha(c) || (c == '\'' && index > 0))
        {
            // Append character to word
            word[index++] = tolower(c);

            // Ignore alphabetical strings too long to be words
            if (index > LENGTH)
            {
                // Consume remainder of alphabetical string
                while (fread(&c, sizeof(char), 1, fdict) && isalpha(c));

                // Prepare for new word
                index = 0;
            }
        }
        else if (index > 0)
        {
            // Terminate current word
            word[index] = '\0';
            // Prepare for next word
            index = 0;

            // Put the word into the table
            ++words;
            unsigned int hashval = hash(word);
            node **curnode;
            curnode = &table[hashval];
            while (*curnode != NULL)
            {
                curnode = &(*curnode)->next;
            }
            *curnode = malloc(sizeof(node));
            strcpy((*curnode)->word, word);
            (*curnode)->next = NULL;
        }
    }

    fclose(fdict);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return words;
}

void free_bucket(node *bucket);
// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    for (unsigned int i = 0; i < N; ++i)
    {
        free_bucket(table[i]);
        table[i] = NULL;
    }
    return true;
}

void free_bucket(node *curnode)
{
    if (!curnode)
    {
        return;
    }
    if (curnode->next)
    {
        free_bucket(curnode->next);
    }
    free(curnode);
}
