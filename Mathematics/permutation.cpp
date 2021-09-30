// 字典序生成n选r的全排列
void permutations(string seq, int len, vector<string>& res) {
    int n = seq.size();
    sort(seq.begin(), seq.end());
    res.emplace_back(seq.substr(0, len));

    vector<int> nxt(len);
    iota(nxt.begin(), nxt.end(), 1);
    while (true) {
        int i;
        for (i = len - 1; i >= 0; --i) {
            while (nxt[i] < n && seq[nxt[i]] == seq[i]) {
                ++nxt[i];
            }
            if (nxt[i] == n) {
                int tmp = seq[i];
                copy(seq.begin() + i + 1, seq.end(), seq.begin() + i);
                seq.back() = tmp;
                nxt[i] = i + 1;
            } else {
                swap(seq[i], seq[nxt[i]++]);
                res.emplace_back(seq.substr(0, len));
                break;
            }
        }
        if (i == -1) break;
    }
}
