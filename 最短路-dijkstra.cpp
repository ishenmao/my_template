#include <bits/stdc++.h>
#include <ext/pb_ds/priority_queue.hpp>
using namespace std;
// dijkstra ��Դ���·
// �����⣺��� P4779 https://www.luogu.org/problemnew/show/P4779 ��ʱ: 469ms / �ڴ�: 10128KB
// ���ȶ��� �Ż�
// ʹ�� pbds �п��޸ĵ����ȶ��У����ö� �� ��Զ�
namespace dijkstra {
    // �����Ԫ�� first ��ʾ����, second �Ǳ��, Ҳ������pair
    struct node{
        int first,second;
        bool operator<(const node& b)const{
            return first > b.first;
        }
    };
    // __gnu_pbds �� pbds �������ռ�
    typedef __gnu_pbds::priority_queue<node> mypri_queue;
    const int maxn = 100005; // ���ڵ�����
    const int inf = 1 << 30;
    // ���ȶ���
    mypri_queue Q;
    // ��������� ���� ids[], ע�⣺�������ָ�������
    mypri_queue::point_iterator ids[maxn]; // point_iterator �ڲ����������ͣ������޸Ķ����ڲ�Ԫ��
    // �ڽӱ�
    vector<node> edge[maxn];
    // ���մ𰸣���Դ�ڵ㵽�����ڵ����̾���
    int dist[maxn], n;
    // ��ʼ�� init, ��Ҫ������� edge
    void init(int nn) {
        n = nn;
        /*
        for(int i = 1; i <= nn; i++) //����ֻ��һ�����ݣ�����Ҫ
            edge[i].clear();
        */
    }
    // �ӱߣ��� (u,v) w Ȩ�أ����� w >= 0
    inline void add_edge(int u, int v, int w) {
        edge[u].emplace_back(node{w, v}); 
        //edge[v].emplace_back(node{w, u}); // ��ǰ�� ������ͼ
    }
    // ��⣬n �ǽڵ������s��Դ�ڵ���
    void solve(int s) {
        //memset(ids, 0, sizeof ids); //��Ϊ��ָ�����飬�ſ���������գ�����ֻ��һ�����ݣ�����Ҫ���
        for(int i = 1; i <= n; i++)
            dist[i] = inf;
        dist[s] = 0;
        // push ������������ �� point_iterator
        ids[s] = Q.push({0, s});
        while(!Q.empty()) {
            int u = Q.top().second; Q.pop();
            for(node nex : edge[u]) {
                int v = nex.second, len = nex.first;
                if(dist[u] + len < dist[v]) {
                    dist[v] = dist[u] + len;
                    if(ids[v] == 0) // ˵����û�����
                        ids[v] = Q.push( node{dist[v], v});
                    else // �Ѿ��ڶӵĻ���ʹ��ָ�����������
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