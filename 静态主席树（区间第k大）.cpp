#include <bits/stdc++.h>
using namespace std;

// ��̬��ϯ������ѯ����� k С
// ԭ������Ҫ��ǰ��ɢ��
namespace kth_tree {
    const int maxn = 100005;
    struct node {
        int lson, rson, par;
        int sum;
    } tree[1800000]; // �ڵ�������ԼΪ n * (log2(n) + 2)
    int root[maxn]; // ÿ���߶���rootָ��, ����ָ���˽�n����
    int L; // �½ڵ��ַ
    int n; // ά�������г���
    // ģ�������ڴ棬p���½ڵ�ĸ���
    inline int newnode(int p) {
        memset(tree + L, 0, sizeof(node));
        tree[L].par = p;
        return L++;
    }
    // ��������
    void build(int id, int l, int r) {
        if(l == r)
            return;
        int mid = (l + r) >> 1;
        tree[id].lson = newnode(id);
        tree[id].rson = newnode(id);
        build(tree[id].lson, l, mid);
        build(tree[id].rson, mid + 1, r);
    }
    // �������, �����½ڵ��ַ
    int update(int id, int new_par, int l, int r, int x) {
        if(l == r) {
            int new_leaf = newnode(new_par);
            tree[new_leaf].sum = tree[id].sum + 1;
            return new_leaf;
        }
        int mid = (l + r) >> 1, ls = tree[id].lson, rs = tree[id].rson;
        int new_ls = ls, new_rs = rs;
        int new_self = newnode(new_par);
        if(x <= mid) {
            tree[new_self].rson = tree[id].rson;
            new_ls = update(ls, new_self, l, mid, x);
            tree[new_self].lson = new_ls;
        } else {
            tree[new_self].lson = tree[id].lson;
            new_rs = update(rs, new_self, mid + 1, r, x);
            tree[new_self].rson = new_rs;
        }
        tree[new_self].sum = tree[new_ls].sum + tree[new_rs].sum;
        return new_self;
    }
    // ��ʼ��, �������� ��β��ַ��������ǰ��ɢ��
    void init(int* begin, int* end) {
        L = 0;
        n = end - begin;
        root[0] = newnode(-1);
        // ��һ�ÿ���
        build(root[0], 1, n);
        // ��һ��������Ȩֵ����Ӧλ�ü�1
        for(int i = 1; i <= n; i++) {
            root[i] = update(root[i - 1], -1, 1, n, begin[i - 1]);
        }
    }
    // base_query
    int base_query(int lid, int rid, int l, int r, int k) {
        if(l == r)
            return l;
        int lson_cnt = tree[tree[rid].lson].sum - tree[tree[lid].lson].sum;
        int mid = (l + r) >> 1;
        if(lson_cnt >= k)
            return base_query(tree[lid].lson, tree[rid].lson, l, mid, k);
        return base_query(tree[lid].rson, tree[rid].rson, mid + 1, r, k - lson_cnt);
    }
    // �ش�ָ������ �� k С����СֵΪ��1С
    int query(int l, int r, int k) {
        base_query(root[l - 1], root[r], 1, n, k);
    }
}
// ����
int main() {
    int n = 10;
    int a[] = {1, 3, 3, 5, 2, 7, 7, 1, 4, 6};
    for(int i = 1; i <= n; i++) {
        arr[i] = a[i - 1];
    }
    kth_tree::init(n);
    int l, r, k;
    while(cin >> l >> r >> k) {
        int res = kth_tree::query(l, r, k);
        cout << "the " << k << "th small number of [" << l << ", " << r << "] is ";
        cout << res << endl;
    }
    /*
    1 1 1
    the 1th small number of [1, 1] is 1
    1 2 1
    the 1th small number of [1, 2] is 1
    1 2 2
    the 2th small number of [1, 2] is 3
    1 5 2
    the 2th small number of [1, 5] is 2
    1 7 5
    the 5th small number of [1, 7] is 5
    5 9 4
    the 4th small number of [5, 9] is 7
    5 9 3
    the 3th small number of [5, 9] is 4
    5 9 2
    the 2th small number of [5, 9] is 2
    5 9 1
    the 1th small number of [5, 9] is 1
    2 6 5
    the 5th small number of [2, 6] is 7

    */
}