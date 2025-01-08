from intcode import Intcode

import sys

if __name__ == '__main__':
    if len(sys.argv) < 2:
        print('Usage: python3 {sys.argv[0]} <program.txt>')
        sys.exit(1)

    with open(sys.argv[1]) as f:
        comp = Intcode(f.read().strip())
    comp.mem[0] = 2

    # Determined by hand.
    main_routine = 'A,B,A,C,B,C,B,A,C,B\n'
    func_a = 'L,6,R,8,R,12,L,6,L,8\n'
    func_b = 'L,10,L,8,R,12\n'
    func_c = 'L,8,L,10,L,6,L,6\n'

    comp.run()

    for c in main_routine:
        comp.read(ord(c))
        comp.run()

    for c in func_a:
        comp.read(ord(c))
        comp.run()

    for c in func_b:
        comp.read(ord(c))
        comp.run()

    for c in func_c:
        comp.read(ord(c))
        comp.run()

    comp.read(ord('n'))
    comp.run()
    comp.read(ord('\n'))

    output = comp.run().strip().split('\n')[-1]
    print(output)
