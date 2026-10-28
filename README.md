# cube-riscv

A cube simulator for riscv. It needs rv64i_zbkx.

# How to compile

```
riscv64-linux-gnu-gcc -march=rv64imacfd_zbkx -static main.c -o main
```

It can run under qemu on other platforms.

# How it works

Given two cubes, `a` and `b`, `op` will return the composition of a then b.
If `a_moves` is the moves to get from a solved cube to `a` and `b_moves` is the same for `b`,
then `op` returns the result of applying `a_moves` then `b_moves` to a solved cube.

The cubes are stored as 4 64-bit values.
These contain the orientation and permutation of the corners and edges as an array of 4 bit values.
Element `i` is at bits `[4 * i + 3 : 4 * i]`.

The permutation contains the position of each piece relative to where it started.
`p[i] = j` means that the piece from position `j` has moved to `i`.

The orientation contains the orientation of each piece.
For edges, this is a 1 bit value that flips whenever the edge is moved by a quarter face turn (half face turns cancel out).
For corners, this is a value in [0,2] that is the rotation from its original orientation in 120 degree steps.
The original orientation has the white/yellow corner face on the same face as the white/yellow center.

`r` and `u` are the result of applying a R or U move to a solved cube.
They are used to apply R or U to any cube by `op(cube, r)`.

The `xperm4` instruction can compose two permutation into one with a single instruction.
It is used to calculate the permutation of the corners and edges.

`xperm4` permutes the 4-bit values in a 64-bit value so it is also used to permute the orientation of `a` before combining it with `b`.
The orientations are then combined with element wise addition modulo 2/3 for the edges/corners.
For edges, addition modulo 2 is equivalent to xor so use that.

For corners, the orientations are first added together.
Since each orientation is in [0,2], adding the two orientations will add them element-wise without one value overflowing into another.
A `xperm4` instruction is then used to map 3 -> 0 and 4 -> 1 to keep the values in the range [0,2].
This is done by `xperm4` with rs1=0x10210.

