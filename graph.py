import numpy as np
import matplotlib.pyplot as plt

x = np.linspace(0, 10)

y1 = []
for i in range(len(x)):
    y1.append(x[i] * (2**x[i]))

y2 = []
for i in range(len(x)):
    y2.append(x[i]**3)

line1 = plt.plot(x, y1, label="exhaustive search")
line2 = plt.plot(x, y2, label="dynamic programming")

plt.ylabel('t')
plt.xlabel('n')
plt.legend()
plt.savefig("graph.png")
# plt.show()