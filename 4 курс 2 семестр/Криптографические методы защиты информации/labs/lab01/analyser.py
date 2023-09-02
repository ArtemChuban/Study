with open("output_50M.txt", "r") as file:
    gamma = file.read()

for i in range(22_361_430 - 10, 22_361_430 + 10):
    piece = gamma[:i]
    pos = gamma.find(piece, i)
    if pos == -1:
        print("Период не найден")
        break
    if pos == i:
        print(f"Период: {i}")
        break

with open("output_period.txt", "r") as file:
    gamma = file.read()
c0 = gamma.count('0')
c1 = gamma.count('1')
print(f"""
Маркировка:
    0: {c0} ({c0 / len(gamma) * 100}%)
    1: {c1} ({c1 / len(gamma) * 100}%)
""")

current_length0 = 0
max_length0 = 0
current_length1 = 0
max_length1 = 0

for symbol in gamma:
    if symbol == '0':
        current_length0 += 1
        if current_length0 > max_length0:
            max_length0 = current_length0
    else:
        current_length0 = 0

    if symbol == '1':
        current_length1 += 1
        if current_length1 > max_length1:
            max_length1 = current_length1
    else:
        current_length1 = 0
if current_length0 > max_length0:
    max_length0 = current_length0
if current_length1 > max_length1:
    max_length1 = current_length1

print(f"""
Максимальные серии подряд:
    0:
        Длина: {max_length0}
        Количество: {gamma.count("0" * max_length0)}
    1:
        Длина: {max_length1}
        Количество: {gamma.count("1" * max_length1)}
""")