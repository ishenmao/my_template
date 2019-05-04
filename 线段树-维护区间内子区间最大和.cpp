#include <bits/stdc++.h>
using namespace std;

// �߶���
// ʾ�����ݣ�
// ά���� �������������˵� ����������
// ά���� ������������Ҷ˵� ����������
// ά���� ���������
// ά���� ������ ������ ������
// �� �������
// ����������£���Ϊ���������Ҫ lmaxsum, rmaxsum, allsum, submaxsum ��Ҫ��ס����Ӧ�� ����
// ע���κ����䶼���ٺ���һ��Ԫ��
namespace seg_tree {
    // �ڵ�������������г��ȵ� 2 �� ��һ��
    const int maxn = 200050;
    int* arr, n; // ԭʼ���� �� ����
    // tree �ڵ�
    struct tree_node {
        int lmaxsum = 0, rmaxsum = 0, allsum = 0, submaxsum = 0; // C++ ֻ�� int��char �����Ͳ���������ʼ��
    } tree[maxn];
    // �ϲ� ������������ �ڵ� x, y �� mix ��ȥ, x ��������ߵģ�y ���ұߵ�
    inline void push_up(tree_node& mix, const tree_node& x, const tree_node& y) {
        // ע��˳������д�� mix �� x ����ͬһ����ʱ ����������� mix �� y ������ͬһ����
        mix.submaxsum = max(x.rmaxsum + y.lmaxsum, max(x.submaxsum, y.submaxsum));
        mix.lmaxsum = max(x.lmaxsum, x.allsum + y.lmaxsum);
        mix.rmaxsum = max(y.rmaxsum, y.allsum + x.rmaxsum);
        mix.allsum = x.allsum + y.allsum;
    }
    // build ����
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
    // init ��ʼ��
    void init(int* begin, int* end) {
        arr = begin - 1; // �� arr ��1����������һ��
        n = end - begin;
        build(1, 1, n);
    }
    // base_query
    tree_node base_query(int id, int l, int r, int ql, int qr) {
        if(ql <= l && r <= qr) // �����ǰ���䱻ѯ������ ��ס���򷵻ص�ǰ id �ڵ�
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
    // �����ѯ������ tree_node �ṹ��
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
    // ������£����� pos λ�õ�����Ϊ x
    inline void update(int pos, int x) {
        base_update(1, 1, n, pos, x);
    }
}
// ����
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
    // ���Ե����޸�
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