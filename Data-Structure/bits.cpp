#include <ctime>
#include <iostream>
#include <map>
#include <string>
#include <vector>
using namespace std;

/* Counting */
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
int ctz1(unsigned x) {
    int res = 0;
    int y;
    y = !(x & 0x0000FFFF) << 4;
    res += y;
    x >>= y;
    y = !(x & 0x000000FF) << 3;
    res += y;
    x >>= y;
    y = !(x & 0x0000000F) << 2;
    res += y;
    x >>= y;
    y = !(x & 0x00000003) << 1;
    res += y;
    x >>= y;
    y = !(x & 0x00000001);
    res += y;
    return res;
}

unsigned ctz2(unsigned x) {
    int res = 32;
    x &= -x;
    if (x & 0x0000FFFF) res -= 16;
    if (x & 0x00FF00FF) res -= 8;
    if (x & 0x0F0F0F0F) res -= 4;
    if (x & 0x33333333) res -= 2;
    if (x & 0x55555555) res -= 1;
    if (x) res -= 1;
    return res;
}

unsigned ctz3(unsigned x) {
    float f = (float)x;
    return (*(uint32_t *)&f >> 23) - 0x7F;
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

int popCount1(uint32_t x) {
    int ret = 0;
    while (x) {
        ++ret;
        x &= x - 1;
    }
    return ret;
}

int popCount2(uint32_t x) {
    x = (x & 0x55555555) + (x >> 1 & 0x55555555);
    x = (x & 0x33333333) + (x >> 2 & 0x33333333);
    x = (x & 0x0F0F0F0F) + (x >> 4 & 0x0F0F0F0F);
    x = (x & 0x00FF00FF) + (x >> 8 & 0x00FF00FF);
    x = (x & 0x0000FFFF) + (x >> 16 & 0x0000FFFF);
    return x;
}

int popCount3(uint32_t x) {
    x -= (x >> 1 & 0x55555555);
    x = (x & 0x33333333) + (x >> 2 & 0x33333333);
    return ((x + (x >> 4)) & 0x0F0F0F0F) % 255;
}

// The best method for counting bits in a 32-bit integer
int popCount4(uint32_t x) {
    x -= (x >> 1 & 0x55555555);
    x = (x & 0x33333333) + (x >> 2 & 0x33333333);
    return ((x + (x >> 4)) & 0x0F0F0F0F) * 0x01010101 >> 24;
}

int popCount5(uint32_t x) {
    x = x - (x >> 1 & 033333333333) - (x >> 2 & 011111111111);
    return ((x + (x >> 3)) & 030707070707) % 63;
}

#define B2(n) n, n + 1, n + 1, n + 2
#define B4(n) B2(n), B2(n + 1), B2(n + 1), B2(n + 2)
#define B6(n) B4(n), B4(n + 1), B4(n + 1), B4(n + 2)
static const unsigned char tab[256] = {B6(0), B6(1), B6(1), B6(2)};

int popCount6(uint32_t x) {
    unsigned char *p = (unsigned char *)&x;
    return tab[p[0]] + tab[p[1]] + tab[p[2]] + tab[p[3]];
}

/* Gray Code */
int binary2GrayCode(int x) { return x ^ (x >> 1); }

int grayCode2Binary1(int x) {
    for (int i = x >> 1; i; i >>= 1) {
        x ^= i;
    }
    return x;
}

int grayCode2Binary2(int x) {
    x ^= x >> 16;
    x ^= x >> 8;
    x ^= x >> 4;
    x ^= x >> 2;
    x ^= x >> 1;
    return x;
}

/* Gosper’s Hack iterates through all n-bit values that have k bits set to 1,
 * from lowest to highest */
int gospersHack(int x, int n) {
    int lo = x & -x;
    int hi = x + lo;
    if (hi >= (1 << n)) return -1;
    return ((hi ^ x) >> 2) / lo | hi;
}

/* From highest to lowest */
int reverseGospersHack(int x) {
	x = ~x;
    int lo = x & -x;
    int hi = x + lo;
    if (hi == 0) return -1;
    return ~(((hi ^ x) >> 2) / lo | hi);
}

/* return val >= x which is a power of 2 */
int up2power(int x) {
    --x;
    x |= x >> 1;
    x |= x >> 2;
    x |= x >> 4;
    x |= x >> 8;
    x |= x >> 16;
    return x + 1;
}

void debug() {
    int nums[10] = {-1, -2147483648, -2147483647, -37, -16,
                    0,  1,           16,          37,  2147483647};
    for (int i = 0; i < 10; ++i) {
        int num = nums[i];
        printf(
            "number %11d: leading zero is %2d, trailing zero is %2d, at least "
            "%2d bits to represent\n",
            num, clz(num), ctz3(num), howManyBits(num));
    }
}

void test() {
    typedef int (*FunPtr)(uint32_t);
    map<FunPtr, string> funs = {
        {popCount1, "popCount1"}, {popCount2, "popCount2"},
        {popCount3, "popCount3"}, {popCount4, "popCount4"},
        {popCount5, "popCount5"}, {popCount6, "popCount6"}};
    for (const auto &item : funs) {
        int T = 200000000;
        FunPtr fun = item.first;
        clock_t start = clock();
        while (T--) {
            fun(0xF0783C1EU);
        }
        clock_t end = clock();
        printf("execution time of %s is %lf s\n", item.second.c_str(),
               1.0 * (end - start) / CLOCKS_PER_SEC);
    }
}

int main() { test(); }
