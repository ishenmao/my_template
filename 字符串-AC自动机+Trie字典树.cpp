#include <bits/stdc++.h>
using namespace std;

// AC自动机(含Trie基础功能)
// 已通过 ：HDU 2222、POJ 2001
// 仅限 全部是 小写字母，允许重复的模式串
namespace trie {
    const int maxn_nodes = 500010;  //字典树的最大节点个数，必须够大
    int next[maxn_nodes][26], fail[maxn_nodes], end[maxn_nodes];
    int prefix_cnt[maxn_nodes]; //统计每个节点的子孙节点中叶子节点个数，即以该节点为前缀的模式串个数，如果不需要基础查询，可舍去
    int root, L;//L：申请的新节点在数组中下标
    int newnode() {
        memset(next[L], 0, sizeof(next[L]));
        end[L] = 0;
        //初始化 prefix_cnt 如果不需要基础查询，可舍去
        prefix_cnt[L] = 0;
        return L++;
    }
    //初始化，使用前请初始化
    void init() {
        L = 0;
        root = newnode();
    }
    //插入模式串 buf，
    void insert(const char* buf) {
        int now = root;
        for(int i = 0; buf[i]; i++) {
            int x = buf[i] - 'a';
            if(next[now][x] == 0)
                next[now][x] = newnode();
            now = next[now][x];
            //更新 prefix_cnt 如果不需要基础查询，可舍去
            prefix_cnt[now]++;
        }
        end[now]++; // 结尾计数
    }
    //删除模式串 buf，务必保证模式串加入过(否则整个自动机乱套)
    void remove(const char* buf) {
        int now = root;
        for(int i = 0; buf[i]; i++) {
            int x = buf[i] - 'a';
            now = next[now][x];
            //更新 prefix_cnt 如果不需要基础查询，可舍去
            prefix_cnt[now]--;
        }
        end[now]--; // 节点没有删除，仅更新了计数
        assert(end[now] >= 0);
    }
    //建立失配指针，支持模式串动态添加、支持动态删除
    //模式串增加后必须重新build，仅删除不需要重新build，或者仅增加旧串也不需要重新build，因为原节点根本没动
    /*
    补充一段注释：
    按层次遍历 trie 树，求出每个结点的危险性和后缀结点，并补齐由它出发的边。危险性与后缀结点
    的求法在【分析】部分第 6、7 自然段已有说明；若当前结点为根结点，则补充的边应指向本身，
    否则若 x 没有 c 孩子，则新建一条这样的边，指向 x 的后缀结点的 c孩子。
    */
    void build() {
        queue<int> Q;
        fail[root] = root;            // root 节点的 后缀节点为 root
        for(int i = 0; i < 26; i++) { // 处理第二层(根节点的孩子)，第二层所有节点的后缀节点为 root
            int son = next[root][i];
            if(son) {
                fail[son] = root;
                Q.push(son);
            }/*else
                next[root][i] = root; // **注释** ： 根节点的空孩子 指向根
            */
        }
        while(!Q.empty()) {
            int now = Q.front();
            Q.pop();
            for(int i = 0; i < 26; i++) {
                int& son = next[now][i]; // 引用，因为下面有赋值
                if(son == 0)
                    son = next[fail[now]][i]; // 关键，将空路径（孩子）补成 自身后缀节点的孩子，一定是深度大的指向浅的，所以自身后缀节点的孩子一定存在
                else {
                    fail[son] = next[fail[now]][i];
                    Q.push(son);
                }
            }
        }
    }
    // 查询（1） 多少个模式串出现在 str 中,
    // 注意 如果模式串 是 abc abc abc def def defg  那么串 abcabcdefdefdef 的统计次数是 5，3个abc，两个def
    // 也就是仅仅统计全部模式串中，有多少个模式串出现了，并不是真正的 str串中出现了几次模式串
    // 重复的模式串也算， 但是重复出现的只算一次
    int query1(const char* str) {
        int now = root;
        int res = 0;
        stack<int> stk;
        for(int i = 0; str[i]; i++) {
            now = next[now][str[i] - 'a'];
            int temp = now;
            while(temp != root) {
                if(end[temp] > 0) {
                    res += end[temp];
                    end[temp] = -end[temp]; //变负表示已经计算过
                    stk.push(temp);
                }
                temp = fail[temp];
            }
        }
        //恢复原先的标记，以备下次查询
        //如果这个 AC自动机 是一次性的，就不需要这个了
        while(!stk.empty()) {
            int temp = stk.top();
            stk.pop();
            end[temp] = -end[temp];
        }
        return res;
    }
    // 查询（2） 真正的 str串中出现了几次模式串，重复的模式串也算， 重复出现的也算
    // 如果模式串 是 abc abc abc def def defg  那么串 abcabcdefdefdef 的统计次数是 12，6个abc，6个def
    int query2(const char* str) {
        int now = root;
        int res = 0;
        for(int i = 0; str[i]; i++) {
            now = next[now][str[i] - 'a'];
            int temp = now;
            while(temp != root) {
                res += end[temp];
                temp = fail[temp];
            }
        }
        return res;
    }
    // 查询（3） 多少个模式串出现在 str 中，重复的模式串只算一个， 重复出现的也只算一个
    // 如果模式串 是 abc abc abc def def defg  那么串 abcabcdefdefdef 的统计次数是 2，1个abc，1个def
    int query3(const char* str) {
        int now = root;
        int res = 0;
        stack<int> stk;
        for(int i = 0; str[i]; i++) {
            now = next[now][str[i] - 'a'];
            int temp = now;
            while(temp != root) {
                if(end[temp] > 0) {
                    res += 1;
                    end[temp] = -end[temp]; //变负表示已经计算过
                    stk.push(temp);
                }
                temp = fail[temp];
            }
        }
        //恢复原先的标记，以备下次查询
        //如果这个 AC自动机 是一次性的，就不需要这个了
        while(!stk.empty()) {
            int temp = stk.top();
            stk.pop();
            end[temp] = -end[temp];
        }
        return res;
    }
    // 查询（4） str串中出现了几次模式串，重复的模式串只算一个， 重复出现的算多次
    // 如果模式串 是 abc abc abc def def defg  那么串 abcabcdefdefdef 的统计次数是 5，2个abc，3个def
    int query4(const char* str) {
        int now = root;
        int res = 0;
        for(int i = 0; str[i]; i++) {
            now = next[now][str[i] - 'a'];
            int temp = now;
            while(temp != root) {
                if(end[temp] > 0)
                    res += 1;
                temp = fail[temp];
            }
        }
        return res;
    }
    //查询
    //字典树基础功能，统计 前缀是 prefix 的模式串有多少个
    //基础功能: 不需要 build
    int query_prefix(const char* prefix) {
        int now = root;
        for(int i = 0; prefix[i]; i++) {
            int x = prefix[i] - 'a';
            int son = next[now][x];
            if(prefix_cnt[son] == 0)
                return 0; //说明没有
            else
                now = son;
        }
        return prefix_cnt[now];
    }
    //查询
    //字典树基础功能，询问 str 是否被插入到字典树中，即完全匹配的单词
    //基础功能: 不需要 build（build后 就不能再 进行完全匹配了）
    bool has(const char* str) {
        int now = root;
        for(int i = 0; str[i]; i++) {
            int x = str[i] - 'a';
            if(next[now][x])
                now = next[now][x];
            else
                return false;
        }
        return end[now] > 0;
    }
    void debug() {
        for(int i = 0; i < L; i++) {
            printf("id = %3d,fail = %3d,end = %3d,chi = [", i, fail[i], end[i]);
            for(int j = 0; j < 26; j++)
                printf("%2d", next[i][j]); printf("]\n");
        }
    }
}
char buf[1000010];
//简单测试
int main() {
    int T;
    int n;
    puts("输入测试组数");
    scanf("%d", &T);
    while(T--) {
        puts("输入模式串个数");
        scanf("%d", &n);//模式串个数
        trie::init();
        printf("输入%d个模式串\n", n);
        for(int i = 0; i < n; i++) {
            scanf("%s", buf);
            trie::insert(buf);
        }
        trie::build();
        puts("输入查询串");
        scanf("%s", buf);
        //------------------------------
        puts("统计多少个模式串出现在查询串中：");
        printf("%d\n", ac.query1(buf));
        puts("统计查询串中出现了几次模式串：");
        printf("%d\n", ac.query2(buf));
        puts("统计多少个模式串出现在查询串中，重复的模式串不算：");
        printf("%d\n", ac.query3(buf));
        puts("统计查询串中出现了几次模式串，重复的模式串不算：");
        printf("%d\n", ac.query4(buf));
        //------------------------------
        puts("输入一个前缀");
        scanf("%s", buf);
        puts("统计多少个模式串包含此前缀：");
        printf("%d\n", ac.query_prefix(buf));
        //------------------------------
        puts("输入一个单词，将查找此单词是否出现过");
        scanf("%s", buf);
        printf("%s\n", ac.has(buf) ? "true" : "false");
        //
        
    }
    return 0;
}