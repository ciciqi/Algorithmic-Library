/* 
 * 后缀数组
 * 实现：倍增 + 快排
 * 时间复杂度：O(n(logn)^2)
 * 空间复杂度：O(n)
 */
// string s end with char '$'
vector<int> suffix_array(const string& s) {
	int n = s.size();
	vector<int> sa(n);
	iota(begin(sa), end(sa), 0);
	vector<int> rk(begin(s), end(s));
	vector<pair<int, int>> key(n);

	for (int k = 1; k < n; k <<= 1) {
		for (int i = 0; i < n; ++i) {
			key[i] = { rk[i], i + k < n ? rk[i + k] : -1 };
		}
		sort(begin(sa), end(sa), [&](int i, int j) { return key[i] < key[j]; });
		
		rk[sa[0]] = 0;
		for (int i = 1; i < n; ++i) {
			rk[sa[i]] = rk[sa[i - 1]] + (key[sa[i]] != key[sa[i - 1]]);
		}
	}

	return move(sa);
}


/* 
 * 后缀数组
 * 实现：倍增 + 计数排序
 * 时间复杂度：O(nlog(n))
 * 空间复杂度：O(n)
 */
void count_sort(vector<int>& arr, const vector<int>& keys) {
	int n = arr.size();
	vector<int> cnts(max(n, 128));
	for (int key: keys) {
		++cnts[key];
	}
	for (int i = 1; i < cnts.size(); ++i) {
		cnts[i] += cnts[i - 1];
	}

	vector<int> narr(n);
	for (int i = n - 1; i >= 0; --i) {
		narr[--cnts[keys[arr[i]]]] = arr[i];
	}
	arr = move(narr);
}

// string s end with char '$'
vector<int> suffix_array(const string& s) {
	int n = s.size();
	vector<int> sa(n), rk(begin(s), end(s)), nrk(n);
	iota(begin(sa), end(sa), 0);
	count_sort(sa, rk);

	auto cmp = [&](int x, int y, int k) -> bool {
		return rk[x] == rk[y] && rk[x + k] == rk[y + k];
	};

	for (int k = 1; k < n; k <<= 1) {
		for (int i = 0; i < n; ++i) {
			sa[i] = (sa[i] - k + n) % n;
		}
		count_sort(sa, rk);

		nrk[sa[0]] = 0;
		for (int i = 1, cnt = 0; i < n; ++i) {
			nrk[sa[i]] = cmp(sa[i], sa[i - 1], k) ? cnt : ++cnt;
		}
		rk.swap(nrk);

		if (rk[sa.back()] == n - 1) {
			break;
		}
	}

	return move(sa);
}


/* 
 * 后缀数组模板
 * 实现：倍增 + 计数排序
 * 时间复杂度: O(nlog(n))
 * 空间复杂度: O(n)
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


