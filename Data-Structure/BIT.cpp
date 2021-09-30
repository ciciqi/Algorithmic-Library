/*
 * 支持区间修改，单点查询
 * 支持单点修改，区间查询
 */
struct BIT: vector<LL> {
    BIT(int n): vector<LL>(n + 1) {}

	BIT(const vector<int>& nums): vector<LL>(nums.begin(), nums.end()) {
        for (int i = 1; i < size(); ++i) {
            int parent = i + (i & -i);
            if (parent < size()) {
                at(parent) += at(i);
            }
        }
    }

    void update(int x, LL delta) {
        for (int i = x; i < size(); i += i & -i) {
            at(i) += delta;
        }
    }

    LL query(int x) {
        LL ret = 0;
        for (int i = x; i; i -= i & -i) {
            ret += at(i);
        }
        return ret;
    }

    int kth(int x) {
        int ret = 0, cnt = 0;
        for (int i = log2(size() - 1); ~i; --i) {
            ret |= 1 << i;
            if (ret < size() && cnt + at(ret) < x) {
                cnt += at(ret);
            } else {
                ret ^= 1 << i;
            }
        }
        return ret + 1;
    }
};

/*
 *	支持区间修改，区间查询
 */
using LL = long long;

struct Data {
    LL s = 0, sx = 0;
    void operator+=(const Data& other) {
        s += other.s;
        sx += other.sx;
    }
};

struct BIT: vector<Data> {
    inline int lsb(int x) {
        return x & -x;
    }
    
    BIT(const vector<LL>& nums): vector<Data>(nums.size()) {
        for (int i = 1; i < size(); ++i) {
            at(i) += Data{ nums[i], i * nums[i] };
            int parent = i + lsb(i);
            if (parent >= size()) continue;
            at(parent) += at(i);
        }
    }
    
    void update(int x, LL delta) {
        Data add{ delta, x * delta };
        for (int i = x; i < size(); i += lsb(i)) {
            at(i) += add;
        }
    }
    
    LL query(int x) {
        Data ret;
        for (int i = x; i; i -= lsb(i)) {
            ret += at(i);
        }
        return (x + 1) * ret.s - ret.sx;
    }
};


/*
 * 支持二维区间修改，区间查询
 */
using LL = long long;

struct Data {
    LL s = 0, sx = 0, sy = 0, sxy = 0;
    void operator+=(const Data& other) {
        s += other.s;
        sx += other.sx;
        sy += other.sy;
        sxy += other.sxy;
    }
};

struct BIT: vector<vector<Data>> {
    BIT(int n, int m): vector<vector<Data>>(n + 1, vector<Data>(m + 1)) {}
    
    void update(int x, int y, LL delta) {
        Data data = { delta, x * delta, y * delta, x * y * delta };
        for (int i = x; i < size(); i += i & -i) {
            for (int j = y; j < at(i).size(); j += j & -j) {
                at(i)[j] += data;
            }
        }
    }
    
    LL query(int x, int y) {
        Data data;
        for (int i = x; i; i -= i & -i) {
            for (int j = y; j; j -= j & -j) {
                data += at(i)[j];
            }
        }
        return (x + 1) * (y + 1) * data.s - (y + 1) * data.sx - (x + 1) * data.sy +
            data.sxy;
    }
};
