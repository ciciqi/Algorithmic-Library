template <typename T>
struct Greater {
    bool operator()(const T& a, const T& b) { return a > b; }
};

template <typename T, typename Cmp>
class Heap {
   private:
    vector<T> arr;
    int size;
    Cmp cmp;

    inline int getLeft(int x) { return x << 1; }

    inline int getRight(int x) { return (x << 1) + 1; }

    inline int getParent(int x) { return x >> 1; }

    void up(int x) {
        for (int fa = getParent(x); fa > 0 && cmp(arr[fa], arr[x]);
             fa = getParent(x)) {
            swap(arr[fa], arr[x]);
            x = fa;
        }
    }

    void down(int x) {
        while (true) {
            int left = getLeft(x);
            int right = getRight(x);
            int nxt = x;
            if (left <= size && cmp(arr[nxt], arr[left])) nxt = left;
            if (right <= size && cmp(arr[nxt], arr[right])) nxt = right;
            if (nxt == x) break;
            swap(arr[nxt], arr[x]);
            x = nxt;
        }
    }

   public:
    Heap(int n) : size(0) {
        arr.reserve(n);
        arr.resize(1);
    }

    void push(T x) {
        arr.emplace_back(x);
        ++size;
        up(size);
    }

    void pop() {
        swap(arr[1], arr.back());
        arr.pop_back();
        --size;
        down(1);
    }

    const T& top() { return arr[1]; }

    bool empty() { return size == 0; }
};
