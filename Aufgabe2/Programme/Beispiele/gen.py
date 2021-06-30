import random
import base64

n = 10000
m = 1000

fruits = ["".join(chr(ord('A')+int(j)) for j in str(i)) for i in range(0, m)]
random.shuffle(fruits)
observe = []


for i in range(n):
    k = random.randint(2, m)
    observe.append([(str(j+1), fruits[j]) for j in random.sample(range(0, m), k)])

k = random.randint(2, m)
d = [fruits[j] for j in random.sample(range(0, m), k)]

# print(observe)
print(m)
print(" ".join(d))
print(n)
for i in range(n):
    print(" ".join([j[0] for j in observe[i]]))
    print(" ".join([j[1] for j in observe[i]]))
