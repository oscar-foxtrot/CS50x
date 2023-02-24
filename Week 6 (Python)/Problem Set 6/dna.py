import csv
import sys


def main():

    # Checking for command-line usage
    if (len(sys.argv) != 3) or (sys.argv[1][-4:] != '.csv'):
        print('Usage: python dna.py data.csv sequence.txt')
        return 1

    # Open the files
    csvfile = open(sys.argv[1])
    txtfile = open(sys.argv[2])

    # Reading database file into a variable
    csvrdr = csv.DictReader(csvfile)

    # Reading DNA sequence file into a variable
    sample = txtfile.readline()
    txtfile.close()

    # Finding longest match of each STR in DNA sequence
    sampleseqs = dict()
    csvfile.seek(5)
    tmps = csvfile.readline()
    csvfile.seek(0)
    if tmps[-1:] == '\n':
        tmps = tmps[:-1]
    tmps = tmps.split(sep=',')
    for i in tmps:
        sampleseqs.update({i: '0'})

    dictcp = dict()
    for key in sampleseqs:
        dictcp.update({key: str(longest_match(sample, key))})
    sampleseqs = dictcp

    # Check database for matching profiles
    matched_name = None
    for entry in csvrdr:
        matched = True
        for key in sampleseqs:
            if sampleseqs[key] != entry[key]:
                matched = False
                break
        if matched:
            matched_name = entry['name']
            break

    if not bool(matched_name):
        print('No match')
    else:
        print(matched_name)

    csvfile.close()
    return


def longest_match(sequence, subsequence):
    """Returns length of longest run of subsequence in sequence."""

    # Initialize variables
    longest_run = 0
    subsequence_length = len(subsequence)
    sequence_length = len(sequence)

    # Check each character in sequence for most consecutive runs of subsequence
    for i in range(sequence_length):

        # Initialize count of consecutive runs
        count = 0

        # Check for a subsequence match in a "substring" (a subset of characters) within sequence
        # If a match, move substring to next potential match in sequence
        # Continue moving substring and checking for matches until out of consecutive matches
        while True:

            # Adjust substring start and end
            start = i + count * subsequence_length
            end = start + subsequence_length

            # If there is a match in the substring
            if sequence[start:end] == subsequence:
                count += 1

            # If there is no match in the substring
            else:
                break

        # Update most consecutive matches found
        longest_run = max(longest_run, count)

    # After checking for runs at each character in seqeuence, return longest run found
    return longest_run


main()
