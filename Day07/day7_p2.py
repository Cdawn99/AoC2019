from intcode import Intcode

import sys
from copy import deepcopy
from itertools import permutations

if __name__ == '__main__':
    if len(sys.argv) < 2:
        print(f'Usage: python3 {sys.argv[0]} <program.txt>')
        sys.exit(1)

    with open(sys.argv[1]) as f:
        content = f.read().strip()

    comp_base = Intcode(content)

    max_signal = 0
    for phases in permutations(range(5, 10)):
        comps = [deepcopy(comp_base) for _ in range(5)]
        for comp, phase in zip(comps, phases):
            comp.run()
            comp.read(phase)

        signal = 0
        while True:
            for comp in comps:
                comp.run()
                comp.read(signal)
                signal = int(comp.run())
            if not comps[-1].input_required:
                break

        if max_signal < signal:
            max_signal = signal

    print(f'Highest signal: {max_signal}')
