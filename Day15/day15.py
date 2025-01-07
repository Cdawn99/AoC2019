from sys import maxsize


class Direction:
    NORTH = 1
    SOUTH = 2
    WEST = 3
    EAST = 4

    @staticmethod
    def get_dir_vector(d: int):
        match d:
            case Direction.NORTH:
                return (0, -1)
            case Direction.SOUTH:
                return (0, 1)
            case Direction.WEST:
                return (-1, 0)
            case Direction.EAST:
                return (1, 0)
        raise RuntimeError("UNREACHABLE")

    @staticmethod
    def get_dirs():
        return [Direction.EAST, Direction.NORTH, Direction.WEST, Direction.SOUTH]


class Droid:
    def __init__(self):
        self.x = 0
        self.y = 0
        self.map = {}

        self.map[(self.x, self.y)] = 1

    def step(self, command: int, status: int):
        v = Direction.get_dir_vector(command)
        new_x = self.x + v[0]
        new_y = self.y + v[1]
        self.map[(new_x, new_y)] = status
        if status != 0:
            self.x = new_x
            self.y = new_y

    def find_oxygen_system(self):
        """Find the oxygen system and return its location and distance."""
        dist = {i: maxsize for i in self.map.keys()}
        dist[(0, 0)] = 0
        visited = set()
        q = [(0, 0)]
        while len(q) != 0:
            u = q.pop(0)
            for d in Direction.get_dirs():
                v = Direction.get_dir_vector(d)
                n = (u[0] + v[0], u[1] + v[1])
                if self.map[n] == 0 or n in visited:
                    continue
                dist[n] = dist[u] + 1
                if self.map[n] == 2:
                    return n, dist[n]
                q.append(n)
            visited.add(u)

    def minutes_to_fill_with_oxygen(self, oxygen_system: tuple[int, int]) -> int:
        dist = {i: maxsize for i in self.map.keys()}
        dist[oxygen_system] = 0
        visited = set()
        q = [oxygen_system]
        while len(q) != 0:
            u = q.pop(0)
            for d in Direction.get_dirs():
                v = Direction.get_dir_vector(d)
                n = (u[0] + v[0], u[1] + v[1])
                if self.map[n] == 0 or n in visited:
                    continue
                dist[n] = dist[u] + 1
                q.append(n)
            visited.add(u)
        return max(set(i for i in dist.values() if i != maxsize))

    def __str__(self) -> str:
        keys = self.map.keys()

        xs = [i[0] for i in keys]
        ys = [i[1] for i in keys]

        min_x = min(xs)
        max_x = max(xs)
        width = max_x - min_x + 1

        min_y = min(ys)
        max_y = max(ys)
        height = max_y - min_y + 1

        area = [[' ' for _ in range(width)] for _ in range(height)]
        for x, y in zip(xs, ys):
            match self.map[(x, y)]:
                case 0:
                    area[y - min_y][x - min_x] = '#'
                case 1:
                    area[y - min_y][x - min_x] = '.'
                case 2:
                    area[y - min_y][x - min_x] = 'O'

        area = '\n'.join(''.join(i) for i in area)
        return f'Droid position: ({self.x - min_x}, {self.y - min_y})\n{area}'
