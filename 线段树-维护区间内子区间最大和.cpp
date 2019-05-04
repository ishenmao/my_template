#include <bits/stdc++.h>
using namespace std;

// 线段树
// 示例内容：
// 维护了 必须包含区间左端点 的区间最大和
// 维护了 必须包含区间右端点 的区间最大和
// 维护了 整个区间和
// 维护了 区间内 子区间 的最大和
// 含 单点更新
// 不含区间更新，因为区间更新需要 lmaxsum, rmaxsum, allsum, submaxsum 都要记住所对应的 区间
// 注：任何区间都至少含有一个元素
namespace seg_tree {
    // 节点最大数量：序列长度的 2 倍 多一点
    const int maxn = 200050;
    int* arr, n; // 原始序列 和 长度
    // tree 节点
    struct tree_node {
        int lmaxsum = 0, rmaxsum = 0, allsum = 0, submaxsum = 0; // C++ 只有 int、char 等类型才能这样初始化
    } tree[maxn];
    // 合并 两个相邻区间 节点 x, y 到 mix 中去, x 必须是左边的，y 是右边的
    inline void push_up(tree_node& mix, const tree_node& x, const tree_node& y) {
        // 注意顺序，这样写当 mix 和 x 引用同一对象时 不会出错，但是 mix 和 y 不能是同一对象
        mix.submaxsum = max(x.rmaxsum + y.lmaxsum, max(x.submaxsum, y.submaxsum));
        mix.lmaxsum = max(x.lmaxsum, x.allsum + y.lmaxsum);
        mix.rmaxsum = max(y.rmaxsum, y.allsum + x.rmaxsum);
        mix.allsum = x.allsum + y.allsum;
    }
    // build 建树
    void build(int id, int l, int r) {
        if(l == r) {
            tree[id].lmaxsum = tree[id].rmaxsum = tree[id].allsum = tree[id].submaxsum = arr[l];
            return;
        }
        int lson = id << 1, rson = lson + 1, mid = (l + r) >> 1;
        build(lson, l, mid);
        build(rson, mid + 1, r);
        push_up(tree[id], tree[lson], tree[rson]);
    }
    // init 初始化
    void init(int* begin, int* end) {
        arr = begin - 1; // 让 arr 从1索引，后退一个
        n = end - begin;
        build(1, 1, n);
    }
    // base_query
    tree_node base_query(int id, int l, int r, int ql, int qr) {
        if(ql <= l && r <= qr) // 如果当前区间被询问区间 包住，则返回当前 id 节点
            return tree[id];
        int lson = id << 1, rson = lson + 1, mid = (l + r) >> 1;
        tree_node ans;
        if(ql <= mid) {
            ans = base_query(lson, l, mid, ql, qr);
            if(mid < qr)
                push_up(ans, ans, base_query(rson, mid + 1, r, ql, qr));
        } else
            ans = base_query(rson, mid + 1, r, ql, qr);
        return ans;
    }
    // 区间查询，返回 tree_node 结构体
    inline tree_node query(int ql, int qr) {
        return base_query(1, 1, n, ql, qr);
    }

    // base_update
    void base_update(int id, int l, int r, int pos, int x) {
        if(l == r) {
            tree[id].lmaxsum = tree[id].rmaxsum = tree[id].allsum = tree[id].submaxsum = x;
            return;
        }
        int lson = id << 1, rson = lson + 1, mid = (l + r) >> 1;
        if(pos <= mid)
            base_update(lson, l, mid, pos, x);
        else
            base_update(rson, mid + 1, r, pos, x);
        push_up(tree[id], tree[lson], tree[rson]);
    }
    // 单点更新，将第 pos 位置的数改为 x
    inline void update(int pos, int x) {
        base_update(1, 1, n, pos, x);
    }
}
// 测试
int main() {
    int n = 10;
    int a[] = {7, 1, -2, 15, 3, 4, -6, -20, 0, 2};
    seg_tree::init(a, a + n);
    int l, r;
    while(cin >> l >> r, l) {
        auto res = seg_tree::query(l, r);
        cout << "lmaxsum = " << res.lmaxsum << endl;
        cout << "rmaxsum = " << res.rmaxsum << endl;
        cout << "allsum = " << res.allsum << endl;
        cout << "submaxsum = " << res.submaxsum << endl;
    }
    /*
    1 5
    lmaxsum = 24
    rmaxsum = 24
    allsum = 24
    submaxsum = 24
    3 7
    lmaxsum = 20
    rmaxsum = 16
    allsum = 14
    submaxsum = 22
    */
    // 测试单点修改
    int pos, x;
    while(cin >> pos >> x, x) {
        int l, r;
        cin >> l >> r;
        seg_tree::update(pos, x);
        auto res = seg_tree::query(l, r);
        cout << "lmaxsum = " << res.lmaxsum << endl;
        cout << "rmaxsum = " << res.rmaxsum << endl;
        cout << "allsum = " << res.allsum << endl;
        cout << "submaxsum = " << res.submaxsum << endl;
    }
    /*
    0 0
    2 -10
    2 7
    lmaxsum = 10
    rmaxsum = 16
    allsum = 4
    submaxsum = 22
    */
}