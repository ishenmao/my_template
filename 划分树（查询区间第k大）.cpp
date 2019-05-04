#include <bits/stdc++.h>
using namespace std;
typedef long long LL;
// 划分树（partition tree），回答区间第 k 小
// 不支持修改，不需要离散化
// 初始化 O(nlog(n)), 单次询问 log(n)
// 原理：线段树+快速排序
namespace part_tree {
    const int maxn = 100005;
    int n; // 序列个数
    int sorted[maxn]; // 已排序数组
    int tree[18][maxn]; // tree[0][] 为初始数组
    int num[18][maxn]; // 计数第i层各子序列 有多少个数落入自己左儿子
    // 建树
    void build(int l, int r, int row) {
        if(l == r) // 当 l == r 时, 这个分支已经处理好
            return;
        int mid = (l + r) >> 1;
        // 重要！ isame 表示在当前 [l, r] 中有多少个等于 sorted[mid] 的数落入左儿子
        int isame = mid - l + 1; // 左儿子最大长度 mid - l + 1
        for(int i = l; i <= r; i++) {
            if(tree[row][i] < sorted[mid])
                isame--; // 只需要减去 小于 sorted[mid] 的数量
        }
        int ln = l, rn = mid + 1; // 左右儿子序列起始位置
        for(int i = l; i <= r; i++) {
            // 如果是开头，初始 0，否则初始为上一个值
            num[row][i] = i == l ? 0 : num[row][i - 1];
            // isame > 0 保证左右均衡的关键
            if(tree[row][i] < sorted[mid] || tree[row][i] == sorted[mid] && isame > 0) {
                tree[row + 1][ln++] = tree[row][i];
                num[row][i]++;
                if(tree[row][i] == sorted[mid])
                    isame--;
            } else {
                tree[row + 1][rn++] = tree[row][i];
            }
        }
        build(l, mid, row + 1);
        build(mid + 1, r, row + 1);
    }
    // 初始化
    void init(int* start, int* end) {
        n = end - start;
        memcpy(tree[0] + 1, start, sizeof(int)*n); // 快速复制数组
        memcpy(sorted + 1, start, sizeof(int)*n); // 快速复制数组
        sort(sorted + 1, sorted + 1 + n);
        build(1, n, 0);
    }
    // base_query
    int base_query(int l, int r, int row, int ql, int qr, int k) {
        if(l == r)
            return tree[row][l];
        int mid = (l + r) >> 1;
        // [l, ql - 1]内落入左儿子个数
        int left_front = ql == l ? 0 : num[row][ql - 1];
        // [ql, qr]内落入左儿子个数
        int left_cnt = num[row][qr] - left_front;
        if(left_cnt >= k) {
            int new_ql = l + left_front;
            int new_qr = new_ql + left_cnt - 1; //注意 减1
            return base_query(l, mid, row + 1, new_ql, new_qr, k);
        } else {
            int new_ql = mid + 1 + ql - l - left_front;
            int new_qr = new_ql + qr - ql + 1 - left_cnt - 1;
            return base_query(mid + 1, r, row + 1, new_ql, new_qr, k - left_cnt);
        }
    }
    // 回答指定区间第 k 小值, 第1小为最小值
    int query(int l, int r, int k) {
        return base_query(1, n, 0, l, r, k);
    }
}
// 测试
int main() {
    int n = 10;
    int a[] = {2, 1, 6, 9, 6, 12, 14, 6, 2, 12};
    part_tree::init(a, a + 10);
    int l, r, k;
    while(cin >> l >> r >> k) {
        int res = part_tree::query(l, r, k);
        cout << "the " << k << "th small number of [" << l << ", " << r << "] is ";
        cout << res << endl;
    }
    /*
    1 3 3
    the 3th small number of [1, 3] is 6
    2 5 3
    the 3th small number of [2, 5] is 6
    4 10 2
    the 2th small number of [4, 10] is 6
    4 10 7
    the 7th small number of [4, 10] is 14
    4 10 5
    the 5th small number of [4, 10] is 12
    3 9 5
    the 5th small number of [3, 9] is 9
    */
}