#include <bits/stdc++.h>
using namespace std;
typedef long long LL;
namespace rmq {
    //typedef pair<int, int> Tp; // ����Ҫά�� ��ֵ �� ��ֵλ�� ʱ���� pair
    typedef int Tp;
    const int maxn = 200005;
    struct Node {
        Tp minval, maxval;
    } dp[maxn][19];
    // 2��18�η�������Լ26W��һ�㹻���������ڴ�󣬲��ܷ��ں����ڲ�
    // ��ʼ��������ԭʼ������β��ַ
    void init(Tp* begin, Tp* end) {
        int n = end - begin;
        for(int i = 1; i <= n; i++) {
            dp[i][0].maxval = dp[i][0].minval = *(begin++);
        }
        for(int j = 1; (1 << j) <= n; j++) {
            for(int i = 1; i + (1 << j) - 1 <= n; i++) {
                int x = i + (1 << (j - 1));
                dp[i][j].maxval = max(dp[i][j - 1].maxval, dp[x][j - 1].maxval);
                dp[i][j].minval = min(dp[i][j - 1].minval, dp[x][j - 1].minval);
            }
        }
    }
    // ���䶼�Ǵ�1��ʼ
    Tp max_of(int l, int r) {
        int x = 31 - __builtin_clz(r - l + 1);
        return max(dp[l][x].maxval, dp[r + 1 - (1 << x)][x].maxval);
    }
    Tp min_of(int l, int r) {
        int x = 31 - __builtin_clz(r - l + 1);
        return min(dp[l][x].minval, dp[r + 1 - (1 << x)][x].minval);
    }
}
// ����
int main() {
    
}