#include <iostream>

using namespace std;

// count leading zero
unsigned clz(unsigned x) {
    unsigned res = 32;
    for (unsigned i = 4, offset; ~i; --i) {
        offset = !!(x >> (1 << i)) << i;
        res -= offset;
        x >>= offset;
    }
    res -= !!x;
    return res;
}

// count trailing zero
unsigned ctz(unsigned x) {
    unsigned res = 0;
    for (unsigned i = 4, offset, mask; ~i; --i) {
        mask = (1 << (1 << i)) - 1;
        offset = !(x & mask) << i;
        res += offset;
        x >>= offset;
    }
    res += !x;
    return res;
}

// return the minimum number of bits required to represent x in two's complement
unsigned howManyBits(int x) {
    x ^= (x >> 31);
    unsigned res = 0;
    for (unsigned i = 4, offset; ~i; --i) {
        offset = !!(x >> (1 << i)) << i;
        res += offset;
        x >>= offset;
    }
    res += !!x + 1;
    return res;
}

void debug() {
    int nums[10] = {-1, -2147483648, -2147483647, -37, -16,
                    0,  1,           16,          37,  2147483647};
    for (int i = 0; i < 10; ++i) {
        int num = nums[i];
        printf(
            "number %11d: leading zero is %2d, trailing zero is %2d, at least "
            "%2d bits to represent\n",
            num, clz(num), ctz(num), howManyBits(num));
    }
}

int main() { debug(); }
