# save 640 values of 40 in binary (without library)

import random

with open('line_position.bin', 'w') as f:
    for i in range(640):
        f.write(chr(random.randint(0, 40)))
