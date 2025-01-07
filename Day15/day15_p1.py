from intcode import Intcode
from day15 import Droid, Direction

import sys


def get_player_command(prompt: str) -> Direction:
    command = input(prompt)
    while True:
        match command:
            case 'w':
                return Direction.NORTH
            case 'a':
                return Direction.WEST
            case 's':
                return Direction.SOUTH
            case 'd':
                return Direction.EAST
            case _:
                continue


if __name__ == '__main__':
    if len(sys.argv) < 2:
        print(f'Usage: python3 {sys.argv[0]} <program.txt>')
        sys.exit(1)

    with open(sys.argv[1]) as f:
        comp = Intcode(f.read().strip())

    droid = Droid()

    print('\033[2J\033[H', end='')

    comp.run()
    print(droid)

    command = Direction.SOUTH
    while comp.input_required:
        # command = get_player_command('Input command (wasd): ')
        comp.read(command)
        status = int(comp.run().strip())
        droid.step(command, status)
        print('\033[2J\033[H', end='')
        print(droid)
        if status == 0:
            match command:
                case Direction.SOUTH:
                    command = Direction.EAST
                case Direction.EAST:
                    command = Direction.NORTH
                case Direction.NORTH:
                    command = Direction.WEST
                case Direction.WEST:
                    command = Direction.SOUTH
        if status == 1:
            match command:
                case Direction.SOUTH:
                    command = Direction.WEST
                case Direction.WEST:
                    command = Direction.NORTH
                case Direction.NORTH:
                    command = Direction.EAST
                case Direction.EAST:
                    command = Direction.SOUTH
        if droid.x == 0 and droid.y == 0 and command == Direction.SOUTH:
            break

    _, steps_to_oxygen = droid.find_oxygen_system()
    print(f'Steps to oxygen: {steps_to_oxygen}')
