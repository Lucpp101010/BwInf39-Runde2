import random

fn = "reell"
u = 10.5
n = 10

# houses = random.sample(range(0, u), n)
houses = [random.random()*u for i in range(0, n)]
houses.sort()

f = open(fn, "w")
f.write(str(u) + " " + str(n) + "\n")
f.write(" ".join([str(i) for i in houses]))