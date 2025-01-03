from intcode import Intcode

import sys
from time import sleep

SLEEP_DUR = 0.01


class Joystick:
    LEFT = -1
    NEUTRAL = 0
    RIGHT = 1


class Player:
    def __init__(self):
        self._joystick = Joystick.NEUTRAL

    def read_input(self, prompt: str):
        player_input = input(prompt)
        match player_input:
            case 'a':
                self._joystick = Joystick.LEFT
            case 's':
                self._joystick = Joystick.NEUTRAL
            case 'd':
                self._joystick = Joystick.RIGHT

    @property
    def joystick(self) -> Joystick:
        return self._joystick

    @joystick.setter
    def joystick(self, val: Joystick):
        self._joystick = val


class TileId:
    EMPTY = 0
    WALL = 1
    BLOCK = 2
    PADDLE = 3
    BALL = 4


class Tiles:
    def __init__(self):
        self.tiles = {}
        self.player_score = 0

    def parse_tiles_info(self, info: str):
        info = info.strip().split('\n')
        for i in range(0, len(info), 3):
            x = int(info[i])
            y = int(info[i + 1])
            tile_id = int(info[i + 2])
            if x == -1 and y == 0:
                self.player_score = tile_id
                continue
            self.tiles[(x, y)] = tile_id

    def paddle_to_ball_dir(self) -> Joystick:
        paddle = None
        ball = None
        for pos, tile_id in self.tiles.items():
            match tile_id:
                case TileId.PADDLE:
                    paddle = pos
                case TileId.BALL:
                    ball = pos
        xdiff = ball[0] - paddle[0]
        if (xdiff != 0):
            xdiff /= abs(xdiff)
        return xdiff

    def __str__(self) -> str:
        keys = self.tiles.keys()

        xs = [i[0] for i in keys]
        ys = [i[1] for i in keys]

        min_x = min(xs)
        max_x = max(xs)
        width = max_x - min_x + 1

        min_y = min(ys)
        max_y = max(ys)
        height = max_y - min_y + 1

        screen = [[' ' for _ in range(width)] for _ in range(height)]
        for x, y in zip(xs, ys):
            match self.tiles[(x, y)]:
                case TileId.EMPTY:
                    screen[y - min_y][x - min_x] = ' '
                case TileId.WALL:
                    screen[y - min_y][x - min_x] = '#'
                case TileId.BLOCK:
                    screen[y - min_y][x - min_x] = 'B'
                case TileId.PADDLE:
                    screen[y - min_y][x - min_x] = 'P'
                case TileId.BALL:
                    screen[y - min_y][x - min_x] = 'O'

        screen = '\n'.join(''.join(i) for i in screen)
        return f'{self.player_score}\n{screen}'


if __name__ == '__main__':
    if len(sys.argv) < 2:
        print(f'Usage: python3 {sys.argv[0]} <program.txt>')
        sys.exit(1)

    with open(sys.argv[1]) as f:
        code = f.read().strip()

    comp = Intcode(code)
    comp.mem[0] = 2

    player = Player()
    tiles = Tiles()

    print('\033[2J\033[H', end='')

    output = comp.run()
    tiles.parse_tiles_info(output)
    print(tiles)
    sleep(SLEEP_DUR)
    while comp.input_required:
        # player.read_input('Move joystick (left = a, neutral = enter, right = d): ')
        player.joystick = tiles.paddle_to_ball_dir()
        comp.read(player.joystick)
        output = comp.run()
        tiles.parse_tiles_info(output)
        print('\033[2J\033[H', end='')
        print(tiles)
        sleep(SLEEP_DUR)

    print(f'Final score: {tiles.player_score}')
