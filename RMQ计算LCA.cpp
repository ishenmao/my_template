#include <bits/stdc++.h>
using namespace std;
typedef long long LL;
typedef pair<int,int> Tp;
namespace rmq {
    const int maxn = 200005;
    struct Node {
        Tp minval, maxval;
    } dp[maxn][19];
    // 2��18�η�������Լ26W��һ�㹻���������ڴ�󣬲��ܷ��ں����ڲ�
    // ��ʼ��������ԭʼ����
    // ע�� dp�����±�� 1 ��ʼ
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

// ʹ�� RMQ ���� LCA
// �� ŷ����ŷ������
// ���ڵ��� 1-n��ʹ���ڽӱ�洢

namespace lca {
    const int maxn = 100005;
    vector<int>* edge; // �ڽӱ�
    Tp  euler_ord[2 * maxn]; // ���������Ϣ�ͱ��
    int ne = 0; // ���� 2n-1, ��1��ʼ�洢
    int par[maxn]; //˳���¼���ڵ㣬���ܻ�����
    int pos[maxn]; //��¼���Ϊ i �Ľڵ��� euler_ord[] ���±꣬����¼��һ�ν���ʱ��λ��
    
    // Ԥ������ŷ����
    void dfs(int u, int depth = 0, int p = -1) {
        par[u] = p;
        pos[u] = ++ne; // �ȼ� ne ����1��ʼ
        euler_ord[ne].first = depth;
        euler_ord[ne].second = u;
        for(int v : edge[u]) {
            if(v != p) {
                dfs(v, depth + 1, u);
                euler_ord[++ne].first = depth;
                euler_ord[ne].second = u;
            }
        }
    }
    
    // ��ʼ��
    void init(vector<int>* e, int n, int root = 1) {
        edge = e, ne = 0;
        dfs(root);
        rmq::init(euler_ord + 1, euler_ord + 1 + ne);
    }
    // �ش� u, v �� lca
    int query(int u, int v) {
        int l = pos[u], r = pos[v];
        if(l > r)
            swap(l, r);
        return rmq::min_of(l, r).second;
    }
    
}
int main() {
    cout << sizeof(pair<int,int>) << endl;
    int n;
    cin >> n;
    vector<int> edge[20];
    int nn = n;
    while(--nn){
        int a,b;
        cin >> a >> b;
        edge[a].push_back(b);
        edge[b].push_back(a);
    }
    lca::init(edge, n, 1);
    int qu, qv;
    while(cin >> qu >> qv){
        cout << "lca of " << qu << " " << qv << " is ";
        cout << lca::query(qu, qv) << endl;
    }
}