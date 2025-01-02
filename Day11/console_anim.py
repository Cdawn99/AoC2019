from intcode import Intcode
from day11 import Robot

import sys
from time import sleep

SLEEP_TIME = 0.01

if __name__ == '__main__':
    if len(sys.argv) < 2:
        print(f'Usage: python3 {sys.argv[0]} <program.txt>')
        sys.exit(1)

    with open(sys.argv[1]) as f:
        code = f.read().strip()

    comp = Intcode(code)
    robo = Robot()

    print('\033[2J\033[H', end='')
    sleep(SLEEP_TIME)

    # Comment the following two lines out to draw Part 1.
    robo.set_panel(1)
    print(robo)

    comp.run()
    while comp.input_required:
        comp.read(robo.get_pos_col())
        output = comp.run()
        color, turn_direction = map(int, output.strip().split('\n'))
        robo.step(color, turn_direction)
        print('\033[H', end='')
        print(robo)
        sleep(SLEEP_TIME)
