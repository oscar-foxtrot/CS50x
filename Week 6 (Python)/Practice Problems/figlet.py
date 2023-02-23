import sys
import random
import pyfiglet

if (len(sys.argv) not in [1, 3]):
    print('Invalid usage')
    exit(1)

fig = pyfiglet.Figlet()
fts = fig.getFonts()
if (len(sys.argv) == 3):
    if (sys.argv[2] not in fts) or (sys.argv[1] not in ['-f', '--font']):
        print('Invalid usage')
        exit(1)
    else:
        fig.setFont(font=sys.argv[2])
else:
    fig.setFont(font=fts[random.randint(0, len(fts) - 1)])

inp = input('Text: ')
print(fig.renderText(inp))
