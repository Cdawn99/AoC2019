from intcode import Intcode
from day11 import Robot
import sys

if __name__ == '__main__':
    if len(sys.argv) < 2:
        print(f'Usage: python3 {sys.argv[0]} <program.txt>')
        sys.exit(1)

    with open(sys.argv[1]) as f:
        code = f.read().strip()

    comp = Intcode(code)
    robo = Robot()

    comp.run()
    while comp.input_required:
        comp.read(robo.get_pos_col())
        output = comp.run()
        color, turn_direction = map(int, output.strip().split('\n'))
        robo.step(color, turn_direction)

    print(f'Number of panels painted at least once: {len(robo.painted_panels)}')
