#include <bits/stdc++.h>
using namespace std;
// 单模匹配 kmp 算法, str 被查找串，长度 n, mode 模式串, 长度 m
// 查询 次数、查询位置
namespace kmp {
    const int maxn = 100005;
    int next[maxn]; // 模式串的前缀 的 border 长度, next[i] := mode[0 to i] 的 border 长度，i 最大为 mlen - 1
    char* mode; // 模式串
    int mlen; // 模式串长度
    // 初始化 mode ： 模式串，长度 m，预处理next数组
    void init(char* m_str, int m) {
        mode = m_str, mlen = m, next[0] = 0;
        int i = 1, j = 0;
        while(i < m) {
            while(j > 0 && mode[i] != mode[j])
                j = next[j - 1];
            mode[i] == mode[j] ? next[i++] = ++j : next[i++] = 0;
        }
    }
    // 查询 mode 在给定文本串 text 中第一次出现的位置，没有则返回 -1
    inline int query_pos(const char* text, int n = -1) {
        n = n == -1 ? strlen(text) : n;
        int i = 0, j = 0;
        while(i < n && j < mlen) {
            if(text[i] == mode[j])
                ++i, ++j;
            else
                j == 0 ? ++i : j = next[j - 1];
        }
        return j == mlen ? i - mlen : -1;
    }
    // 查询 mode 在给定文本串 text 中一共出现的次数
    inline int query_count(const char* text, int n = -1) {
        n = n == -1 ? strlen(text) : n;
        int i = 0, j = 0, ans = 0;
        while(i < n) {
            if(text[i] == mode[j])
                ++i, ++j;
            else
                j == 0 ? ++i : j = next[j - 1];
            if(j == mlen)
                j = next[j - 1], ans++;
        }
        return ans;
    }

}

int main() {
    char buf[500], text[500];
    while(cin >> buf >> text) { // abababaabababbaa
        int len = strlen(buf);
        kmp::init(buf, len);
        cout << kmp::query_pos(text) << endl;
        cout << kmp::query_count(text) << endl;
        cout << endl;
    }
    /*
    ababacab ababacacababacab
    8
    1

    abc     ababaccabcabcabcabcabca
    7
    5

    c acacacaddd
    1
    3


    */
    return 0;
}