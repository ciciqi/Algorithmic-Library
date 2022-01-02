struct Node {
    int children[26]{};
    int fail = 0;
    int height = 0;
    unordered_set<int> inv_fail;
    bool end = false;

    int& operator[](char ch) { return children[ch - 'a']; }
};

/*
 * AC自动机
 * reference:
 * - https://oi-wiki.org/string/ac-automaton/
 * - http://se.ethz.ch/~meyer/publications/string/string_matching.pdf
 */
class AC {
private:
    static constexpr int N  = 100007;
    int tot;
    Node nodes[N];

    void inverse_failure(int s, int new_fail, char ch) {
        for (const auto& inv: nodes[s].inv_fail) {
            int inv_child = nodes[inv][ch];
            if (nodes[inv].height + 1 == nodes[inv_child].height) {
                nodes[nodes[inv_child].fail].inv_fail.erase(inv_child);
                nodes[inv_child].fail = new_fail;
                nodes[new_fail].inv_fail.insert(inv_child);
            } else {
                nodes[inv][ch] = new_fail;
                inverse_failure(inv, new_fail, ch);
            }
        }
    }

    void enter_child(int cur, int nxt, char ch) {
        nodes[nxt].height = nodes[cur].height + 1;
        nodes[nxt].fail = nodes[nodes[cur].fail][ch];
        nodes[nodes[nxt].fail].inv_fail.insert(nxt);

        // 语句"nodes[cur][ch] = nxt;"，必须在结点nxt的fail连接完毕之后执行
        // 否则，当nodes[cur].fail == cur时（如编号为0的根结点），结点nxt的fail会指向自身nxt
        // 因为结点本身不为该结点的longest proper suffix(lps)，所以这个fail指针的连接是错误的
        nodes[cur][ch] = nxt;

        inverse_failure(cur, nxt, ch);

        // 语句"nodes[cur][ch] = nxt;"，必须在结点nxt复制结点fail的children之前执行
        // 否则，当fail == cur时（或者说fail、cur都是根结点），fail的children不会被nxt及时更新，
        // 从而导致结点nxt复制到的children不完全
        // 注意这里采用了空子结点也充当fail指针的策略
        int fail = nodes[nxt].fail;
        for (char letter = 'a'; letter <= 'z'; ++letter) {
            nodes[nxt][letter] = nodes[fail][letter];
        }
    }

public:
    AC(): tot(1) {}

    // 支持字符串的在线增量插入，实现字符串的插入、字符串的fail指针连接，
    // 以及其它结点的fail指针更新
    // 时间复杂度：O(n * |s|)，n表示树的结点数量，|s|表示插入字符串的长度
    // 在inverse_failure()中最坏情况下要对每个已存在的结点进行访问
    // 空间复杂度：O(h)，h表示树的高度，在inverse_failure()中最坏情况下的递归深度为h
    void incremental_insert(const string& s) {
        int state = 0;
        for (const auto& ch: s) {
            int nxt_state = nodes[state][ch];
            if (nodes[nxt_state].height != nodes[state].height + 1) {
                nxt_state = tot++;
                enter_child(state, nxt_state, ch);
            }
            state = nxt_state;
        }
        nodes[state].end = true;
    }

    // 实现字符串的离线插入
    // 时间复杂度：O(|s|)
    // 空间复杂度：O(1)
    void insert(const string& s) {
        int state = 0;
        for (const auto& ch: s) {
            int nxt_state = nodes[state][ch];
            if (nxt_state == 0) {
                nxt_state = tot++;
                nodes[nxt_state].height = nodes[state].height + 1;
                nodes[state][ch] = nxt_state;
            }
            state = nxt_state;
        }
        nodes[state].end = true;
    }

    // 依据结点从小到大的高度顺序，实现对每个结点的fail指针连接
    // 时间复杂度：O(C * n)，C = 26表示字符集大小，n表示树中的结点数量
    // 空间复杂度：O(n)
    void build() {
        queue<int> Q;
        for (char ch = 'a'; ch <= 'z'; ++ch) {
            if (nodes[0][ch] != 0) {
                Q.emplace(nodes[0][ch]);
            }
        }
        while (!Q.empty()) {
            int u = Q.front();
            Q.pop();
            for (char ch = 'a'; ch <= 'z'; ++ch) {
                int nxt = nodes[u][ch];
                if (nxt) {
                    nodes[nxt].fail = nodes[nodes[u].fail][ch];
                    Q.emplace(nxt);
                } else {
                    nodes[u][ch] = nodes[nodes[u].fail][ch];
                }
            }
        }
    }
};

