#include <iostream>
#include <string>
#include <cstring>
using namespace std;

/* 
 * Suffix Array with Prefix doubling algorithm
 * Time complexity: O(nlog(n))
 * Space complexity: O(n)
 */
template<class T = string, int N = 100007, int MAX = 256>
struct SuffixArray {
    T seq;
    int n, mem1[N << 1], mem2[N << 1];
    int sa[N], *rk = mem1, *oldrk = mem2, second[N], temp[N], cnts[N] = {0};
    int height[N];

    bool cmp(int x, int y, int w) {
        return oldrk[x] == oldrk[y] && oldrk[x + w] == oldrk[y + w];
    }

    SuffixArray(const T& _seq, int _n): seq(_seq), n(_n) {
        int m = MAX;
        rk[n] = oldrk[n] = -1;
        for (int i = 0; i < n; ++i) ++cnts[rk[i] = seq[i]];
        for (int i = 1; i < m; ++i) cnts[i] += cnts[i - 1];
        for (int i = n - 1; i >= 0; --i) sa[--cnts[rk[i]]] = i;

        for (int w = 1; w < n; w <<= 1) {
            int idx = 0;
            for (int i = n - w; i < n; ++i) second[idx++] = i;
            for (int i = 0; idx < n; ++i) if (sa[i] >= w) second[idx++] = sa[i] - w;

            memset(cnts, 0, sizeof(int) * m);
            for (int i = 0; i < n; ++i) ++cnts[temp[i] = rk[second[i]]];
            for (int i = 1; i < m; ++i) cnts[i] += cnts[i - 1];
            for (int i = n - 1; i >= 0; --i) sa[--cnts[temp[i]]] = second[i];

            idx = -1;
            swap(rk, oldrk);
            for (int i = 0; i < n; ++i) {
                rk[sa[i]] = i > 0 && cmp(sa[i - 1], sa[i], w) ? idx : ++idx;
            }
            if (idx + 1 == n) break;
            m = idx + 1;
        }
    }

    void getHeight() {
        height[0] = 0;
        for (int i = 0, k = 0; i < n; ++i) {
            if (k) --k;
            if (rk[i] < 1) continue;
            int j = sa[rk[i] - 1];
            while (i + k < n && j + k < n && seq[i + k] == seq[j + k]) ++k;
            height[rk[i]] = k;
        }
    }

    void toString() {
        int n = seq.size();
        printf("suffix array:\n");
        for (int i = 0; i < n; ++i) {
            printf("%d ", sa[i]);
        }
        putchar('\n');
        printf("height array:\n");
        for (int i = 0; i < n; ++i) {
            printf("%d ", height[i]);
        }
        putchar('\n');
        printf("rank array:\n");
        for (int i = 0; i < n; ++i) {
            printf("%d ", rk[i]);
        }
        putchar('\n');
    }
};


/*
 * AC自动机
 * 时间复杂度：均摊O(m + Cm + n)，最坏O(m + Cm + n^2)
 * 空间复杂度：O(Cm)
 * n表示文本串的长度，m为模式串总字符个数，C为字符集大小
 */
struct Node {
    int nxt[26]{};
    int fail = 0;
    int id = -1;
    int len = 0;
    int& operator[](size_t i) {
        return nxt[i];
    }
};

struct AC {
    static constexpr int N = 1e5 + 7;
    Node nodes[N];
    int size = 1;

    void insert(const string& s, int id) {
        int u = 0;
        for (char c: s) {
            int nxt = c - 'a';
            if (nodes[u][nxt] == 0) {
                nodes[u][nxt] = size++;
            }
            u = nodes[u][nxt];
        }
        nodes[u].id = id;
        nodes[u].len = s.length();
    }

    void build() {
        queue<int> Q;
        for (int i = 0; i < 26; ++i) {
            if (nodes[0][i]) Q.emplace(nodes[0][i]);
        }
        while (!Q.empty()) {
            int u = Q.front();
            Q.pop();
            for (int i = 0; i < 26; ++i) {
                int nxt = nodes[u][i];
                if (nxt) {
                    nodes[nxt].fail = nodes[nodes[u].fail][i];
                    Q.emplace(nxt);
                } else {
                    nodes[u][i] = nodes[nodes[u].fail][i];
                }
            }
        }
    }

    vector<vector<int>> search(const string& s, int n) {
        vector<vector<int>> ret(n);
        int u = 0;
        for (int i = 0; i < s.length(); ++i) {
            int nxt = s[i] - 'a';
            u = nodes[u][nxt];
            for (int j = u; j; j = nodes[j].fail) {
                if (nodes[j].id != -1) {
                    ret[nodes[j].id].emplace_back(i - nodes[j].len + 1);
                }
            }
        }
        return move(ret);
    }
};

int main() {
	return 0;
}
