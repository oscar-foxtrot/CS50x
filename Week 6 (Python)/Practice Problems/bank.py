s = input('Greeting: ').lstrip()
if s.lower()[0:5] == 'hello':
    print('$0')
elif s[0:1].lower() == 'h':
    print('$20')
else:
    print('$100')
