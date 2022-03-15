#include <bits/stdc++.h>
using namespace std;
// 最长回文子串，马拉车算法
// 复杂度 线性
namespace manacher {
    const int maxn = 200005; // 查询串长度的2倍多
    char ma[maxn] = {}; //加了间隔符后的字符串，就用 0 填充
    int mp[maxn]; // ma 的回文半径, mp[0] = mp[1] = 1，中心也算在半径里面
    int n, maxlen; // n: 扩充串 ma 的长度, maxlen: 原串最大回文子串长度
    // 初始化，传入 字符串 s
    inline void init(const char* s, int len) {
        n = 2, maxlen = -1; // ma 串的长度，临时当下标用
        for(int i = 0; i < len; i++, n += 2)
            ma[n] = s[i];
        //此时 n == 2*len + 2
        //但实际需要 2*len + 3 的空间
        //末尾放置 '$'， 方便下面 while 退出循环。
        ma[0] = '#', ma[n] = '$'; // 首尾补成不一样字符，'$'不计算
        // 计算回文半径
        int id = 0, mx = 1; // id: 目前最远回文串中心，mx = id + mp[id]，即 以id为中心的回文串最右不能到达位置
        mp[0] = 1;
        for(int i = 1; i < n; i++) {
            mp[i] = mx > i ? min(mp[2 * id - i], mx - i) : 1;
            int l = i - mp[i], r = i + mp[i];
            while(ma[l--] == ma[r++])
                mp[i]++;
            if(i + mp[i] > mx)
                id = i, mx = i + mp[i];
            maxlen = max(maxlen, mp[i] - 1);
        }
    }
    // 回答最长回文子串，在原串中起始下标, 多个符合要求的返回第一个
    int query_index() {
        int id = 2;
        while(mp[id] != maxlen + 1)
            id++;
        return (id - mp[id]) >> 1;
    }
}
// 测试
int main() {
    char buf[1000];
    while(cin >> buf) {
        manacher::init(buf, strlen(buf));
        cout << manacher::maxlen << endl;
        cout << "index is " << manacher::query_index() << endl;
    }
    /*
    ababc
    3
    index is 0
    a
    1
    index is 0
    bccc
    3
    index is 1
    babcbbc
    4
    index is 3
    */
}
