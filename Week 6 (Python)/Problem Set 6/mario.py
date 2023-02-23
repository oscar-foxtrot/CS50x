import cs50

while True:
    hgt = cs50.get_int('Height: ')
    if not (hgt == None) and (hgt >= 1) and (hgt <= 8):
        break

for i in range(hgt):
    print(f"{' ' * (hgt - i - 1)}{(i + 1) * '#'}  {(i + 1) * '#'}")
