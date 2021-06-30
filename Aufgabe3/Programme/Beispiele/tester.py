import random
import subprocess as sub
import time

samples = 100
u = 10 ** 9
n = 100

while n <= 1000000:
    stabil = 0
    totalTime = 0
    for i in range(0, samples):
        houses = random.sample(range(0, u), n)
        houses.sort()

        start = time.time()

        p = sub.Popen("./g", stdin=sub.PIPE, stdout=sub.PIPE)
        p.stdin.write((str(u) + " " + str(n) + "\n").encode())
        p.stdin.write(b" ".join([str(h).encode() for h in houses]) + b"\n")
        p.stdin.flush()

        ans = p.stdout.readline().decode("UTF-8").strip()

        end = time.time()
        totalTime += end - start

        if(not ans.startswith("Es wurde keine")):
            stabil += 1
        p.kill()

    print("n=%d"%n)
    print("Stabile Verteilungen:", stabil)
    print("Durchschnittliche Zeit: ", totalTime / samples)
    n *= 10
