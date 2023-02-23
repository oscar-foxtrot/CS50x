import cs50


def main():
    txt = cs50.get_string("Text: ")
    if len(txt) == 0:
        return 0
    lets = count_letters(txt)
    wrds = count_words(txt)
    sents = count_sentences(txt)
    index = round((0.0588 * lets / wrds * 100) - (0.296 * sents / wrds * 100) - 15.8)

    if index < 1:
        print("Before Grade 1\n")
    elif index >= 16:
        print("Grade 16+\n")
    else:
        print(f"Grade {index}\n")

    return 0


# Count the number of letters in |txt|
def count_letters(txt):
    '''
    txt: str: the entered text
    returns: int: number of letters
    '''
    count = 0
    for i in range(len(txt)):
        c = txt[i]
        if ((c <= 'z' and c >= 'a') or (c <= 'Z' and c >= 'A')):
            count += 1
    return count


# Count the number of '.', '!' and '?' in |txt|
def count_sentences(txt):
    '''
    txt: str: the entered text
    returns: int: number of sentences
    '''
    count = 0
    for i in range(len(txt)):
        c = txt[i]
        if c == '.' or c == '?' or c == '!':
            count += 1
    return count


# Count the number of sequences of letters in |txt| (based on the special symbols: ',', '.' etc)
def count_words(txt):
    '''
    txt: str: the entered text
    returns: int: number of words
    '''
    count = 0
    for i in range(len(txt)):
        c1 = txt[i - 1]
        c2 = txt[i]
        if c1.isalpha() and \
            (c2 == ',' or c2 == '.' or c2 == ';' or c2 == ':' or c2 == '?' or c2 == '!'
             or c2 == '$' or c2 == '\"' or c2 == ')' or c2 == '(' or c2 == ' '
             or c2 == '{' or c2 == '}' or c2 == '[' or c2 == ']'):
            count += 1
    return count


main()
