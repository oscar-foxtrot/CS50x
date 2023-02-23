def main():
    jar1 = Jar(10)
    jar2 = Jar(20)
    jar1.deposit(10)
    jar1.withdraw(7)
    jar2.deposit(2)
    print(jar1) #prints 3 cookie emojis
    print(jar2) #prints 2 cookie emoji


class Jar:
    def __init__(self, capacity=12):
        if type(capacity) != int:
            raise ValueError
        Jar.capacity = 12
        self.capacity = capacity
        Jar.size = 0

    def __str__(self):
        return '🍪' * self.size

    def deposit(self, n):
        if self.size + n > self.capacity:
            raise ValueError
        self.size += n
        return

    def withdraw(self, n):
        if self.size - n < 0:
            raise ValueError
        self.size -= n
        return

    @property
    def capacity(self):
        return self.capacity

    @property
    def size(self):
        return self.size


main()
