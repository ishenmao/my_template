#include<bits/stdc++.h>
typedef long long LL;
using namespace std;
// 并查集: union find set
// 仅路径压缩，不考虑按秩合并（因为秩不精确且提升微弱）
// 节点编号 1 - n
// 均摊复杂度：大约线性
namespace ufset {
    const int maxn = 200005;
    int par[maxn];// 父亲数组，注：root 的 父亲 保存的是所在集合大小，为区分其他编号，用负数保存
    void init(int n) {
        memset(par + 1, -1, sizeof(int)*(n)); // 初始每个元素就是 1 个集合，个数为1，初始化成 -1
    }
    // 查找根
    int find(int x) {
        return par[x] < 0 ? x : par[x] = find(par[x]);
    }
    // 合并两个节点所在集合，如果合并之前就已经是同一集合，返回false
    inline bool link(int x, int y) {
        x = find(x), y = find(y);
        if(x == y)
            return false;
        par[y] += par[x]; //加上 x 所在集合个数
        par[x] = y; // 把 x 并到 y 上
        return true;
    }
    // 返回 x 所在集合 个数
    inline int size(int x){
        return -par[find(x)];
    }
}
// 测试：太简单，略
int main() {
    int n;
}
