import re


class Intcode:
    def __init__(self, code: str):
        self.ip = 0
        self.mem = [int(i[0]) for i in re.finditer(r'-?\d+', code)]

        self.input_required = False

    def run(self) -> str:
        """Run until either the program is done, or an input is required.

        To check if an input is required, query the `input_required` member.
        in case it is `True`, call the `read()` method to provide an input.

        """
        output = ''
        while self.mem[self.ip] != 99:
            match self.mem[self.ip] % 100:
                case 1:
                    self.ip += self._add(self.mem[self.ip] // 100)
                case 2:
                    self.ip += self._mul(self.mem[self.ip] // 100)
                case 3:  # READ
                    self.input_required = True
                    break
                case 4:
                    shift, out = self._print(self.mem[self.ip] // 100)
                    output += out
                    self.ip += shift
                case 5:
                    self.ip = self._jnz(self.mem[self.ip] // 100)
                case 6:
                    self.ip = self._jz(self.mem[self.ip] // 100)
                case 7:
                    self.ip += self._lt(self.mem[self.ip] // 100)
                case 8:
                    self.ip += self._eq(self.mem[self.ip] // 100)
        return output

    def read(self, n: int):
        assert self.mem[self.ip] % 100 == 3
        self.mem[self.mem[self.ip + 1]] = n
        self.ip += 2
        self.input_required = False

    def _add(self, arg_modes: int) -> int:
        s = 0
        for ip in range(self.ip + 1, self.ip + 3):
            mode = arg_modes % 10
            if mode == 1:
                s += self.mem[ip]
            else:
                s += self.mem[self.mem[ip]]
            arg_modes //= 10
        self.mem[self.mem[self.ip + 3]] = s
        return 4

    def _mul(self, arg_modes: int) -> int:
        m = 1
        for ip in range(self.ip + 1, self.ip + 3):
            mode = arg_modes % 10
            if mode == 1:
                m *= self.mem[ip]
            else:
                m *= self.mem[self.mem[ip]]
            arg_modes //= 10
        self.mem[self.mem[self.ip + 3]] = m
        return 4

    def _print(self, arg_mode: int) -> int:
        if arg_mode == 1:
            return 2, f'{self.mem[self.ip + 1]}\n'
        return 2, f'{self.mem[self.mem[self.ip + 1]]}\n'

    def _jnz(self, arg_modes: int) -> int:
        params = []
        for i in range(1, 3):
            if arg_modes % 10 == 1:
                params.append(self.mem[self.ip + i])
            else:
                params.append(self.mem[self.mem[self.ip + i]])
            arg_modes //= 10
        return params[1] if params[0] != 0 else self.ip + 3

    def _jz(self, arg_modes: int) -> int:
        params = []
        for i in range(1, 3):
            if arg_modes % 10 == 1:
                params.append(self.mem[self.ip + i])
            else:
                params.append(self.mem[self.mem[self.ip + i]])
            arg_modes //= 10
        return params[1] if params[0] == 0 else self.ip + 3

    def _lt(self, arg_modes: int) -> int:
        params = []
        for i in range(1, 3):
            if arg_modes % 10 == 1:
                params.append(self.mem[self.ip + i])
            else:
                params.append(self.mem[self.mem[self.ip + i]])
            arg_modes //= 10
        self.mem[self.mem[self.ip + 3]] = 1 if params[0] < params[1] else 0
        return 4

    def _eq(self, arg_modes: int) -> int:
        params = []
        for i in range(1, 3):
            if arg_modes % 10 == 1:
                params.append(self.mem[self.ip + i])
            else:
                params.append(self.mem[self.mem[self.ip + i]])
            arg_modes //= 10
        self.mem[self.mem[self.ip + 3]] = 1 if params[0] == params[1] else 0
        return 4

    def __repr__(self) -> str:
        s = ','.join(str(i) for i in self.mem)
        return f'Intcode(\'{s}\')'

    def __str__(self) -> str:
        return f'ip = {self.ip}\nmem = {self.mem}'
