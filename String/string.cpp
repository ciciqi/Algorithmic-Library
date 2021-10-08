#include <iostream>
#include <string>
#include <cstring>
using namespace std;


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
