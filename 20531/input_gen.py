import random

inp = list(map(int, input().split()))
n = inp[0]
m = inp[1]

with open("input.txt", "w") as file:
    file.write(f"{n} {m}\n")
    for i in range(m):
        s1 = random.randrange(1, n+1)
        s2 = random.randrange(1, n+1)
        file.write(f"{s1} {s2}\n")