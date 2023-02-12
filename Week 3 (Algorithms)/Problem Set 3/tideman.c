#include <stdio.h>
#include <string.h>
#include <cs50.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
}
pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    if (pair_count != 0)
    {
        sort_pairs();
    }
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    for (int i = 0; i < candidate_count; ++i)
    {
        if (strcmp(name, candidates[i]) == 0)
        {
            ranks[rank] = i;
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    for (int i = 0; i < candidate_count - 1; ++i)
    {
        for (int j = i + 1; j < candidate_count; ++j)
        {
            ++preferences[ranks[i]][ranks[j]];
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    pair_count = 0;     // it's not (candidate_count - 1) * candidate count / 2, because a tie is a possibility
    for (int i = 0; i < candidate_count - 1; ++i)
    {
        for (int j = i + 1; j < candidate_count; ++j)
        {
            if (preferences[i][j] > preferences[j][i])
            {
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;
                ++pair_count;
            }
            else if (preferences[i][j] < preferences[j][i])
            {
                pairs[pair_count].winner = j;
                pairs[pair_count].loser = i;
                ++pair_count;
            }
        }
    }
    return;
}

void msort(int beg, int end); // MERGE SORT

void sort_pairs(void)
{
    msort(0, pair_count - 1);
    return;
}

// Sort pairs in decreasing order by strength of victory (using MERGE SORT)
void msort(int beg, int end)
{
    if (beg == end)
    {
        return;
    }
    int fhend = (end + beg) / 2; //first half's last elem (number)

    msort(beg, fhend);
    msort(fhend + 1, end);

    pair tmp[end - beg + 1];
    int cur1 = beg, cur2 = fhend + 1, cntr = 0;

    while (cur1 <= fhend && cur2 <= end)
    {
        // The following line determines whose victory is "stronger".
        if (preferences[pairs[cur1].winner][pairs[cur1].loser] < preferences[pairs[cur2].winner][pairs[cur2].loser])
        {
            tmp[cntr].winner = pairs[cur2].winner;
            tmp[cntr].loser = pairs[cur2].loser;
            ++cur2;
        }
        else
        {
            tmp[cntr].winner = pairs[cur1].winner;
            tmp[cntr].loser = pairs[cur1].loser;
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
            tmp[cntr].winner = pairs[cur1].winner;
            tmp[cntr].loser = pairs[cur1].loser;
            ++cur1;
            ++cntr;
        }
    }

    for (int i = 0; i < cntr; ++i)
    {
        pairs[i + beg].winner = tmp[i].winner;
        pairs[i + beg].loser = tmp[i].loser;
    }
    return;
}

// Find a possible cycle recursively
int remove_cycle(int i, int j, int src)
{
    if (locked[i][src])
    {
        return 1;
    }
    else
    {
        for (int k = 0; k < candidate_count; ++k)
        {
            if (locked[j][k])
            {
                if (remove_cycle(j, k, src))
                {
                    return 1;
                }
            }
        }
    }
    return 0;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    for (int i = 0; i < pair_count; ++i)
    {
        locked[pairs[i].winner][pairs[i].loser] = true; // lock in an arrow from the winner to the loser
        if (remove_cycle(pairs[i].winner, pairs[i].loser, pairs[i].winner))
        {
            locked[pairs[i].winner][pairs[i].loser] = false;
        } // remove the arrow from the graph created by the previous line in case it created a cycle
    }
    return;
}

// Print ALL winners of the election
void print_winner(void)
{
    for (int i = 0; i < candidate_count; ++i)
    {
        int src = 1; // is the element the source of the graph? (flag)
        for (int j = 0; j < candidate_count; ++j)
        {
            if (locked[j][i])
            {
                src = 0;
                break;
            }
        }
        if (src)
        {
            printf("%s\n", candidates[i]);
        }
    }
    return;
}
