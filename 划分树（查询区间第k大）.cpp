#include <bits/stdc++.h>
using namespace std;
typedef long long LL;
// ��������partition tree�����ش������ k С
// ��֧���޸ģ�����Ҫ��ɢ��
// ��ʼ�� O(nlog(n)), ����ѯ�� log(n)
// ԭ���߶���+��������
namespace part_tree {
    const int maxn = 100005;
    int n; // ���и���
    int sorted[maxn]; // ����������
    int tree[18][maxn]; // tree[0][] Ϊ��ʼ����
    int num[18][maxn]; // ������i��������� �ж��ٸ��������Լ������
    // ����
    void build(int l, int r, int row) {
        if(l == r) // �� l == r ʱ, �����֧�Ѿ������
            return;
        int mid = (l + r) >> 1;
        // ��Ҫ�� isame ��ʾ�ڵ�ǰ [l, r] ���ж��ٸ����� sorted[mid] �������������
        int isame = mid - l + 1; // �������󳤶� mid - l + 1
        for(int i = l; i <= r; i++) {
            if(tree[row][i] < sorted[mid])
                isame--; // ֻ��Ҫ��ȥ С�� sorted[mid] ������
        }
        int ln = l, rn = mid + 1; // ���Ҷ���������ʼλ��
        for(int i = l; i <= r; i++) {
            // ����ǿ�ͷ����ʼ 0�������ʼΪ��һ��ֵ
            num[row][i] = i == l ? 0 : num[row][i - 1];
            // isame > 0 ��֤���Ҿ���Ĺؼ�
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
    // ��ʼ��
    void init(int* start, int* end) {
        n = end - start;
        memcpy(tree[0] + 1, start, sizeof(int)*n); // ���ٸ�������
        memcpy(sorted + 1, start, sizeof(int)*n); // ���ٸ�������
        sort(sorted + 1, sorted + 1 + n);
        build(1, n, 0);
    }
    // base_query
    int base_query(int l, int r, int row, int ql, int qr, int k) {
        if(l == r)
            return tree[row][l];
        int mid = (l + r) >> 1;
        // [l, ql - 1]����������Ӹ���
        int left_front = ql == l ? 0 : num[row][ql - 1];
        // [ql, qr]����������Ӹ���
        int left_cnt = num[row][qr] - left_front;
        if(left_cnt >= k) {
            int new_ql = l + left_front;
            int new_qr = new_ql + left_cnt - 1; //ע�� ��1
            return base_query(l, mid, row + 1, new_ql, new_qr, k);
        } else {
            int new_ql = mid + 1 + ql - l - left_front;
            int new_qr = new_ql + qr - ql + 1 - left_cnt - 1;
            return base_query(mid + 1, r, row + 1, new_ql, new_qr, k - left_cnt);
        }
    }
    // �ش�ָ������� k Сֵ, ��1СΪ��Сֵ
    int query(int l, int r, int k) {
        return base_query(1, n, 0, l, r, k);
    }
}
// ����
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