#include <functional>
#include <iostream>
#include <random>
#include <string>
#include <vector>

using namespace std;

/* Rabin-Karp algorithm with rolling hash */
struct RabinKarp {
    using LL = long long;
    int mod;
    int base;

    RabinKarp(pair<int, int> range = {1e6, 1e7}, int _mod = 1e9 + 7)
        : mod(_mod) {
        mt19937 gen(random_device{}());
        auto dis = uniform_int_distribution<int>(range.first, range.second);
        base = dis(gen);
    }

    template <class T>
    vector<int> getHash(const T& seq, int len) {
        const int n = seq.size();
        if (len > n) return {};
        vector<int> ret;
        ret.reserve(n - len + 1);
        int mul = 1;
        int hash = 0;
        for (int i = 0; i < len; ++i) {
            hash = ((LL)hash * base % mod + seq[i]) % mod;
            mul = (LL)mul * base % mod;
        }
        ret.push_back(hash);
        for (int i = len; i < n; ++i) {
            hash = ((LL)hash * base % mod - (LL)seq[i - len] * mul % mod +
                    seq[i]) %
                   mod;
            if (hash < 0) hash += mod;
            ret.push_back(hash);
        }
        return move(ret);
    }
};

/*
 * Rabin-Karp algorithm template v2
 */
struct SeqHash {
    using LL = long long;
    LL base, mod;
    vector<LL> hash, power;

    SeqHash(LL _base = 100007, LL _mod = 1e9 + 7) : base(_base), mod(_mod) {}

    template <class T>
    void build(const T& seq, int n) {
        hash = {0};
        power = {1};
        for (int i = 0; i < n; ++i) {
            hash.emplace_back((hash.back() * base % mod + seq[i]) % mod);
            power.emplace_back(power.back() * base % mod);
        }
    }

    LL ask(int left, int right) {
        return (hash[right + 1] - hash[left] * power[right + 1 - left] % mod +
                mod) %
               mod;
    }
};

int main() { return 0; }
