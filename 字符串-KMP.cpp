#include <bits/stdc++.h>
using namespace std;
// ��ģƥ�� kmp �㷨, str �����Ҵ������� n, mode ģʽ��, ���� m
// ��ѯ ��������ѯλ��
namespace kmp {
    const int maxn = 100005;
    int next[maxn]; // ģʽ����ǰ׺ �� border ����, next[i] := mode[0 to i] �� border ���ȣ�i ���Ϊ mlen - 1
    char* mode; // ģʽ��
    int mlen; // ģʽ������
    // ��ʼ�� mode �� ģʽ�������� m��Ԥ����next����
    void init(char* m_str, int m) {
        mode = m_str, mlen = m, next[0] = 0;
        int i = 1, j = 0;
        while(i < m) {
            while(j > 0 && mode[i] != mode[j])
                j = next[j - 1];
            mode[i] == mode[j] ? next[i++] = ++j : next[i++] = 0;
        }
    }
    // ��ѯ mode �ڸ����ı��� text �е�һ�γ��ֵ�λ�ã�û���򷵻� -1
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
    // ��ѯ mode �ڸ����ı��� text ��һ�����ֵĴ���
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