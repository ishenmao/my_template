#include <bits/stdc++.h>
#include <ext/pb_ds/priority_queue.hpp>
using namespace std;
// dijkstra 单源最短路
// 测试题：洛谷 P4779 https://www.luogu.org/problemnew/show/P4779 用时: 469ms / 内存: 10128KB
// 优先队列 优化
// 使用 pbds 中可修改的优先队列，所用堆 是 配对堆
namespace dijkstra {
    // 定义二元组 first 表示距离, second 是编号, 也可以用pair
    struct node{
        int first,second;
        bool operator<(const node& b)const{
            return first > b.first;
        }
    };
    // __gnu_pbds 是 pbds 的命名空间
    typedef __gnu_pbds::priority_queue<node> mypri_queue;
    const int maxn = 100005; // 最大节点数量
    const int inf = 1 << 30;
    // 优先队列
    mypri_queue Q;
    // 定义迭代器 数组 ids[], 注意：这里真的指针迭代器
    mypri_queue::point_iterator ids[maxn]; // point_iterator 内部迭代器类型，用来修改队列内部元素
    // 邻接表
    vector<node> edge[maxn];
    // 最终答案，即源节点到其他节点的最短距离
    int dist[maxn], n;
    // 初始化 init, 主要用于清空 edge
    void init(int nn) {
        n = nn;
        /*
        for(int i = 1; i <= nn; i++) //本题只有一组数据，不需要
            edge[i].clear();
        */
    }
    // 加边，边 (u,v) w 权重，必须 w >= 0
    inline void add_edge(int u, int v, int w) {
        edge[u].emplace_back(node{w, v}); 
        //edge[v].emplace_back(node{w, u}); // 当前题 是有向图
    }
    // 求解，n 是节点个数，s是源节点编号
    void solve(int s) {
        //memset(ids, 0, sizeof ids); //因为是指针数组，才可以这样清空，本题只有一组数据，不需要清空
        for(int i = 1; i <= n; i++)
            dist[i] = inf;
        dist[s] = 0;
        // push 函数返回类型 是 point_iterator
        ids[s] = Q.push({0, s});
        while(!Q.empty()) {
            int u = Q.top().second; Q.pop();
            for(node nex : edge[u]) {
                int v = nex.second, len = nex.first;
                if(dist[u] + len < dist[v]) {
                    dist[v] = dist[u] + len;
                    if(ids[v] == 0) // 说明还没入过队
                        ids[v] = Q.push( node{dist[v], v});
                    else // 已经在队的话，使用指针迭代器更新
                        Q.modify(ids[v], node{dist[v], v});
                }
            }
        }
    }
}
int main() {
    ios::sync_with_stdio(false);
    int n, m, s;
    cin >> n >> m >> s;
    dijkstra::init(n);
    while(m--) {
        int u, v, w;
        cin >> u >> v >> w;
        dijkstra::add_edge(u, v, w);
    }
    dijkstra::solve(s);
    cout << dijkstra::dist[1];
    for(int i = 2; i <= n; i++) {
        cout << " " << dijkstra::dist[i];
    }
}