def main():
    num = int(input('Number: '))
    print(get_issuer(num))


def get_issuer(num):
    '''
    num: card number
    returns: str: name of the issuer company
    '''
    if (not check_validity(num)):
        return 'INVALID'
    else:
        # len = length of the card number (digits)
        len = 2
        while (num > 99):
            len += 1
            num //= 10

        first = num // 10  # first digit of card number
        second = num % 10  # second digit of card number
        if first == 5:
            if second >= 1 and second <= 5 and len == 16:
                return 'MASTERCARD'
            else:
                return 'INVALID'
        elif first == 4:
            if len == 13 or len == 16:
                return 'VISA'
            else:
                return 'INVALID'
        elif first == 3:
            if (second == 4 or second == 7) and len == 15:
                return 'AMEX'
            else:
                return 'INVALID'
        else:
            return 'INVALID'


def check_validity(num):
    '''
    num: card number
    returns: boolean: true if valid, false if invalid
    '''
    # Luhn's algo
    sum = 0
    while (num > 0):
        sum += num % 10
        tmp = (num // 10) % 10 * 2
        sum += tmp // 10 + tmp % 10
        num //= 100
    if sum % 10 == 0:
        return True
    else:
        return False


main()
