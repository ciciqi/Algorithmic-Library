#include <climits>
#include <iostream>
#include <stack>
#include <vector>
using namespace std;

#define PII pair<int, int>

/*
 * Monotonic Stack
 */
// 数组元素作为最大值时的区间范围
vector<PII> maxValRange(const vector<int>& nums) {
    int n = nums.size();
    vector<PII> res(n, PII(0, n - 1));
    stack<int> st;
    for (int i = 0; i < n; ++i) {
        // non-strict less允许重复数组元素作为右边界，
        // 不允许重复元素作为左边界，strict less则反之
        while (!st.empty() && nums[st.top()] <= nums[i]) {
            res[st.top()].second = i - 1;
            st.pop();
        }
        if (!st.empty()) {
            res[i].first = st.top() + 1;
        }
        st.push(i);
    }
    return res;
}

// 另一种写法
vector<PII> maxValRange1(vector<int>& nums) {
    // 加入哨兵，保证栈内的其它元素都会弹出
    nums.push_back(INT_MAX);
    int n = nums.size();
    vector<PII> res(n, PII(0, n - 1));
    stack<int> st;
    for (int i = 0; i < n; ++i) {
        while (!st.empty() && nums[st.top()] <= nums[i]) {
            int idx = st.top();
            st.pop();
            if (!st.empty()) res[idx].first = st.top() + 1;
            res[idx].second = i - 1;
        }
        st.push(i);
    }
    nums.pop_back();
    return res;
}

// 数组元素作为最小值时的区间范围
vector<PII> minValRange(const vector<int>& nums) {
    int n = nums.size();
    vector<PII> res(n, PII(0, n - 1));
    stack<int> st;
    for (int i = 0; i < n; ++i) {
        while (!st.empty() && nums[st.top()] >= nums[i]) {
            res[st.top()].second = i - 1;
            st.pop();
        }
        if (!st.empty()) {
            res[i].first = st.top() + 1;
        }
        st.push(i);
    }
    return res;
}

void debug() {
    vector<int> nums = {5, 2, 3, 8, 0, 7, 9, 1, 4, 6};
    // vector<PII> max_range = maxValRange1(nums);
    vector<PII> max_range = maxValRange(nums);
    vector<PII> min_range = minValRange(nums);
    printf("nums = [");
    for (int i = 0, n = nums.size(); i < n; ++i) {
        printf(",%d" + !i, nums[i]);
    }
    puts("]");
    for (int i = 0, n = nums.size(); i < n; ++i) {
        printf("number %d: index %d, max range [%d, %d], min range [%d, %d]\n",
               nums[i], i, max_range[i].first, max_range[i].second,
               min_range[i].first, min_range[i].second);
    }
}

int main() {
    debug();
    return 0;
}
