import random


class Gamma:
    def __init__(self, f: tuple[int, ...]) -> None:
        self.state = [random.randrange(0, 2) for _ in range(f[0] + 1)]
        self.f = f

    def next(self) -> int:
        new = sum([self.state[i] for i in self.f]) % 2
        self.state.append(new)
        return self.state.pop(0)


g1 = Gamma((19, 18, 17, 14, 0))
g2 = Gamma((14, 5, 3, 1, 0))
g3 = Gamma((8, 4, 3, 2, 0))

counter = 0
limit = 22361430 #int(input("Длина выходной гаммы (в миллионах): "))

output = open(f"output_period.txt", "w")
#limit *= 1e6

print("Генерация...")
while counter < limit:
    a = g1.next()
    b = g2.next()
    c = g3.next()

    if c == 0:
        output.write(str(a))
    else:
        output.write(str(b))

    counter += 1

output.close()
print("Генерация окончена")
