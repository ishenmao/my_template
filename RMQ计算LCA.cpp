#include <bits/stdc++.h>
using namespace std;
typedef long long LL;
typedef pair<int,int> Tp;
namespace rmq {
    const int maxn = 200005;
    struct Node {
        Tp minval, maxval;
    } dp[maxn][19];
    // 2的18次方，长度约26W，一般够长，消耗内存大，不能放在函数内部
    // 初始化，传入原始数组
    // 注： dp数组下标从 1 开始
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
    // 区间都是从1开始
    Tp max_of(int l, int r) {
        int x = 31 - __builtin_clz(r - l + 1);
        return max(dp[l][x].maxval, dp[r + 1 - (1 << x)][x].maxval);
    }
    Tp min_of(int l, int r) {
        int x = 31 - __builtin_clz(r - l + 1);
        return min(dp[l][x].minval, dp[r + 1 - (1 << x)][x].minval);
    }
}

// 使用 RMQ 计算 LCA
// 跑 欧拉序，欧拉序中
// 树节点编号 1-n，使用邻接表存储

namespace lca {
    const int maxn = 100005;
    vector<int>* edge; // 邻接表
    Tp  euler_ord[2 * maxn]; // 保存深度信息和编号
    int ne = 0; // 长度 2n-1, 从1开始存储
    int par[maxn]; //顺便记录父节点，可能会有用
    int pos[maxn]; //记录编号为 i 的节点在 euler_ord[] 中下标，仅记录第一次进入时的位置
    
    // 预处理，跑欧拉序
    void dfs(int u, int depth = 0, int p = -1) {
        par[u] = p;
        pos[u] = ++ne; // 先加 ne ，从1开始
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
    
    // 初始化
    void init(vector<int>* e, int n, int root = 1) {
        edge = e, ne = 0;
        dfs(root);
        rmq::init(euler_ord + 1, euler_ord + 1 + ne);
    }
    // 回答 u, v 的 lca
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