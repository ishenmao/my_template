#include <bits/stdc++.h>
using namespace std;
// ������Ӵ����������㷨
// ���Ӷ� ����
namespace manacher {
    const int maxn = 200005; // ��ѯ�����ȵ�2����
    char ma[maxn] = {}; //���˼��������ַ��������� 0 ���
    int mp[maxn]; // ma �Ļ��İ뾶, mp[0] = mp[1] = 1������Ҳ���ڰ뾶����
    int n, maxlen; // n: ���䴮 ma �ĳ���, maxlen: ԭ���������Ӵ�����
    // ��ʼ�������� �ַ��� s
    inline void init(const char* s, int len) {
        n = 2, maxlen = -1; // ma ���ĳ��ȣ���ʱ���±���
        for(int i = 0; i < len; i++, n += 2)
            ma[n] = s[i];
        ma[0] = '#', ma[n] = '$'; // ��β���ɲ�һ���ַ���'$'������
        // ������İ뾶
        int id = 0, mx = 0; // id: Ŀǰ��Զ���Ĵ����ģ�mx = id + mp[id]���� ��idΪ���ĵĻ��Ĵ����Ҳ��ܵ���λ��
        for(int i = 0; i < n; i++) {
            mp[i] = mx > i ? min(mp[2 * id - i], mx - i) : 1;
            int l = i - mp[i], r = i + mp[i];
            while(ma[l--] == ma[r++])
                mp[i]++;
            if(i + mp[i] > mx)
                id = i, mx = i + mp[i];
            maxlen = max(maxlen, mp[i] - 1);
        }
    }
    // �ش�������Ӵ�����ԭ������ʼ�±�, �������Ҫ��ķ��ص�һ��
    int query_index() {
        int id = 2;
        while(mp[id] != maxlen + 1)
            id++;
        return (id - mp[id]) >> 1;
    }
}
// ����
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