import numpy as np


class Direction:
    NORTH = (0, -1)
    WEST = (-1, 0)
    SOUTH = (0, 1)
    EAST = (1, 0)


class Robot:
    def __init__(self):
        self.x = 0
        self.y = 0
        self.d = Direction.NORTH

        self.painted_panels = set()
        self.panels_state = {}

    def set_panel(self, color: int):
        self.panels_state[(self.x, self.y)] = 1
        self.painted_panels.add((self.x, self.y))

    def get_pos_col(self) -> int:
        if self.panels_state.get((self.x, self.y)) is None:
            self.panels_state[(self.x, self.y)] = 0
        return self.panels_state[(self.x, self.y)]

    def step(self, color: int, turn_direction: int):
        self.panels_state[(self.x, self.y)] = color
        self.painted_panels.add((self.x, self.y))
        if turn_direction == 0:
            self._turn_left()
        else:
            self._turn_right()
        self._move()

    def _move(self):
        self.x += self.d[0]
        self.y += self.d[1]

    def _turn_left(self):
        match self.d:
            case Direction.NORTH:
                self.d = Direction.WEST
            case Direction.WEST:
                self.d = Direction.SOUTH
            case Direction.SOUTH:
                self.d = Direction.EAST
            case Direction.EAST:
                self.d = Direction.NORTH

    def _turn_right(self):
        match self.d:
            case Direction.NORTH:
                self.d = Direction.EAST
            case Direction.WEST:
                self.d = Direction.NORTH
            case Direction.SOUTH:
                self.d = Direction.WEST
            case Direction.EAST:
                self.d = Direction.SOUTH

    def __str__(self):
        keys = self.panels_state.keys()

        xs = np.array([i[0] for i in keys])
        ys = np.array([i[1] for i in keys])

        min_x = xs.min()
        max_x = xs.max()
        width = max_x - min_x + 1

        min_y = ys.min()
        max_y = ys.max()
        height = max_y - min_y + 1

        hull = [['.' for _ in range(width)] for _ in range(height)]
        for x, y in zip(xs, ys):
            hull[y - min_y][x - min_x] = '#' if self.panels_state[(x, y)] == 1 else '.'

        return '\n'.join(''.join(i) for i in hull)
