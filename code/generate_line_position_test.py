# save 640 values of 40 in a comma separated file (without library)

import random

with open('line_position.csv', 'w') as f:
    for i in range(640):
        f.write(str(random.randint(1, 40)))
        if i != 639:
            f.write(',')
        else:
            f.write('\n')
