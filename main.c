#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>

typedef struct {
    uint64_t co;
    uint64_t cp;
    uint64_t eo;
    uint64_t ep;
} cube_t;

/*

Each permutation stores the value that gets mapped to an index at that index
For example,
p = (1 2 3 4 5 6)
    (5 2 1 6 4 3)

p[1] = 3
p[2] = 2
p[3] = 6
p[4] = 5
p[5] = 1
p[6] = 4

*/

cube_t op(cube_t a, cube_t b) {
    cube_t res;
    __asm__ (
        "\n"
        // Corner permutation
        "xperm4 %1, %5, %9 \n"
        // Edge permutation
        "xperm4 %3, %7, %11 \n"

        // Edge orientation
        // Permute orientation a through permutation b
        "xperm4 %2, %6, %11 \n"
        // Combine with orientation b
        "xor %2, %2, %10 \n"

        // Corner orientation
        // Permute orientation a through permutation b
        "xperm4 %0, %4, %9 \n"
        // Combine with orientation b
        "add %0, %0, %8 \n"
        // Modulo 3
        "xperm4 %0, %12, %0 \n"

        : "=&r" (res.co), "=&r" (res.cp), "=&r" (res.eo), "=&r" (res.ep)
        : "r" (a.co), "r" (a.cp), "r" (a.eo), "r" (a.ep), "r" (b.co), "r" (b.cp), "r" (b.eo), "r" (b.ep), "r" (0x10210)
    );

    return res;
}

const cube_t solved = {
    .co = 0,
    .cp = 0xfedcba9876543210,
    .eo = 0,
    .ep = 0xfedcba9876543210,
};

// Corners: 1 -> 3 -> 7 -> 5 -> 1
// Corners: 1=2 3=1 5=1 7=2
// Edges: 3 -> 6 -> 11 -> 7 -> 3
// Edges: 3 6 7 11

const cube_t r = {
    .co = 0x20101020,
    .cp = 0xfedcba9836741250,
    .eo = 0x100011001000,
    .ep = 0xfedc6a98b3547210,
};

// Corners: 0 -> 2 -> 3 -> 1 -> 0
// Corners: 0
// Edges: 0 -> 1 -> 2 -> 3 -> 0
// Edges: 0 1 2 3

const cube_t u = {
    .co = 0,
    .cp = 0xfedcba9876542031,
    .eo = 0x1111,
    .ep = 0xfedcba9876542103,
};

void print_cube(const char *label, cube_t cube)
{
    printf("%s\n", label);
    printf("  co = 0x%" PRIx64 "\n", cube.co);
    printf("  cp = 0x%" PRIx64 "\n", cube.cp);
    printf("  eo = 0x%" PRIx64 "\n", cube.eo);
    printf("  ep = 0x%" PRIx64 "\n", cube.ep);
}

int main() {
    cube_t r2 = op(r, r);
    cube_t r3 = op(r2, r);
    cube_t u2 = op(u, u);
    cube_t u3 = op(u2, u);


    print_cube("Solved", solved);
    print_cube("Solved x Solved", op(solved, solved));
    print_cube("r x Solved", op(r, solved));
    print_cube("Solved x r", op(solved, r));
    print_cube("r x r", op(r, r));
    print_cube("(r x r) x (r x r)", op(r2, r2));

    cube_t alg = solved;
    alg = op(alg, r2);
    alg = op(alg, u3);
    alg = op(alg, r3);
    alg = op(alg, u3);
    alg = op(alg, r);
    alg = op(alg, u);
    alg = op(alg, r);
    alg = op(alg, u);
    alg = op(alg, r);
    alg = op(alg, u3);
    alg = op(alg, r);

    print_cube("Alg (3 edge permutation)", alg);
    print_cube("Alg x Alg", op(alg, alg));
    print_cube("Alg x Alg x Alg", op(op(alg, alg), alg));
}

