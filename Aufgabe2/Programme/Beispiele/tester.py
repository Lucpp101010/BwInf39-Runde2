import random
import subprocess as sub
import time

samples = 50
n = 100
m = 100000
k = 50000

totalTime = 0

data = [(i, str(i)) for i in range(1, m+1)]
for i in range(0, samples):
    
    donald = random.sample(data, k)
    test = []
    for j in range(n):
        test.append(random.sample(data, k))

    start = time.time()

    p = sub.Popen("./y", stdin=sub.PIPE, stdout=sub.PIPE)
    p.stdin.write((str(m) + "\n").encode())
    p.stdin.write(b" ".join([d[1].encode() for d in donald]) + b"\n")
    p.stdin.write((str(n) + "\n").encode())
    for j in range(n):
        p.stdin.write(b" ".join([str(d[0]).encode() for d in test[j]]) + b"\n")
        p.stdin.write(b" ".join([str(d[1]).encode() for d in test[j]]) + b"\n")
    p.stdin.flush()

    ans = p.stdout.readline().decode("UTF-8").strip()
    end = time.time()
    totalTime += end - start

    p.kill()

print("Durchschnittliche Zeit: ", totalTime / samples)
