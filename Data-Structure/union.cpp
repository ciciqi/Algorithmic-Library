struct Union {
    vector<int> fa, size;
    int n, cnt;

    Union(int _n): n(_n), fa(_n), size(_n, 1), cnt(_n) {
        iota(fa.begin(), fa.end(), 0);
    }

    int find(int x) {
        return fa[x] == x ? x : fa[x] = find(fa[x]);
    }

    bool unite(int x, int y) {
        x = find(x);
        y = find(y);
        if (x == y) return false;
        if (size[x] > size[y]) swap(x, y);
        fa[x] = y;
        size[y] += size[x];
        --cnt;
        return true;
    }

    bool connected(int x, int y) {
        return find(x) == find(y);
    }
};
