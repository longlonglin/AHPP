import random
import sys


if __name__ == '__main__':
    data = sys.argv[1]
    n = int(sys.argv[2])
    S = set()
    cnt = 100
    with open(data + '/seeds.txt', 'w') as f:
        while len(S) < cnt:
            x = random.randrange(0, n)
            if x in S:
                continue
            S.add(x)
            f.write(str(x) + '\n')
