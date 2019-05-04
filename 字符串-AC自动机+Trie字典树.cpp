#include <bits/stdc++.h>
using namespace std;

// AC�Զ���(��Trie��������)
// ��ͨ�� ��HDU 2222��POJ 2001
// ���� ȫ���� Сд��ĸ�������ظ���ģʽ��
namespace trie {
    const int maxn_nodes = 500010;  //�ֵ��������ڵ���������빻��
    int next[maxn_nodes][26], fail[maxn_nodes], end[maxn_nodes];
    int prefix_cnt[maxn_nodes]; //ͳ��ÿ���ڵ������ڵ���Ҷ�ӽڵ���������Ըýڵ�Ϊǰ׺��ģʽ���������������Ҫ������ѯ������ȥ
    int root, L;//L��������½ڵ����������±�
    int newnode() {
        memset(next[L], 0, sizeof(next[L]));
        end[L] = 0;
        //��ʼ�� prefix_cnt �������Ҫ������ѯ������ȥ
        prefix_cnt[L] = 0;
        return L++;
    }
    //��ʼ����ʹ��ǰ���ʼ��
    void init() {
        L = 0;
        root = newnode();
    }
    //����ģʽ�� buf��
    void insert(const char* buf) {
        int now = root;
        for(int i = 0; buf[i]; i++) {
            int x = buf[i] - 'a';
            if(next[now][x] == 0)
                next[now][x] = newnode();
            now = next[now][x];
            //���� prefix_cnt �������Ҫ������ѯ������ȥ
            prefix_cnt[now]++;
        }
        end[now]++; // ��β����
    }
    //ɾ��ģʽ�� buf����ر�֤ģʽ�������(���������Զ�������)
    void remove(const char* buf) {
        int now = root;
        for(int i = 0; buf[i]; i++) {
            int x = buf[i] - 'a';
            now = next[now][x];
            //���� prefix_cnt �������Ҫ������ѯ������ȥ
            prefix_cnt[now]--;
        }
        end[now]--; // �ڵ�û��ɾ�����������˼���
        assert(end[now] >= 0);
    }
    //����ʧ��ָ�룬֧��ģʽ����̬��ӡ�֧�ֶ�̬ɾ��
    //ģʽ�����Ӻ��������build����ɾ������Ҫ����build�����߽����Ӿɴ�Ҳ����Ҫ����build����Ϊԭ�ڵ����û��
    /*
    ����һ��ע�ͣ�
    ����α��� trie �������ÿ������Σ���Ժͺ�׺��㣬���������������ıߡ�Σ�������׺���
    �����ڡ����������ֵ� 6��7 ��Ȼ������˵��������ǰ���Ϊ����㣬�򲹳�ı�Ӧָ����
    ������ x û�� c ���ӣ����½�һ�������ıߣ�ָ�� x �ĺ�׺���� c���ӡ�
    */
    void build() {
        queue<int> Q;
        fail[root] = root;            // root �ڵ�� ��׺�ڵ�Ϊ root
        for(int i = 0; i < 26; i++) { // ����ڶ���(���ڵ�ĺ���)���ڶ������нڵ�ĺ�׺�ڵ�Ϊ root
            int son = next[root][i];
            if(son) {
                fail[son] = root;
                Q.push(son);
            }/*else
                next[root][i] = root; // **ע��** �� ���ڵ�Ŀպ��� ָ���
            */
        }
        while(!Q.empty()) {
            int now = Q.front();
            Q.pop();
            for(int i = 0; i < 26; i++) {
                int& son = next[now][i]; // ���ã���Ϊ�����и�ֵ
                if(son == 0)
                    son = next[fail[now]][i]; // �ؼ�������·�������ӣ����� �����׺�ڵ�ĺ��ӣ�һ������ȴ��ָ��ǳ�ģ����������׺�ڵ�ĺ���һ������
                else {
                    fail[son] = next[fail[now]][i];
                    Q.push(son);
                }
            }
        }
    }
    // ��ѯ��1�� ���ٸ�ģʽ�������� str ��,
    // ע�� ���ģʽ�� �� abc abc abc def def defg  ��ô�� abcabcdefdefdef ��ͳ�ƴ����� 5��3��abc������def
    // Ҳ���ǽ���ͳ��ȫ��ģʽ���У��ж��ٸ�ģʽ�������ˣ������������� str���г����˼���ģʽ��
    // �ظ���ģʽ��Ҳ�㣬 �����ظ����ֵ�ֻ��һ��
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
                    end[temp] = -end[temp]; //�为��ʾ�Ѿ������
                    stk.push(temp);
                }
                temp = fail[temp];
            }
        }
        //�ָ�ԭ�ȵı�ǣ��Ա��´β�ѯ
        //������ AC�Զ��� ��һ���Եģ��Ͳ���Ҫ�����
        while(!stk.empty()) {
            int temp = stk.top();
            stk.pop();
            end[temp] = -end[temp];
        }
        return res;
    }
    // ��ѯ��2�� ������ str���г����˼���ģʽ�����ظ���ģʽ��Ҳ�㣬 �ظ����ֵ�Ҳ��
    // ���ģʽ�� �� abc abc abc def def defg  ��ô�� abcabcdefdefdef ��ͳ�ƴ����� 12��6��abc��6��def
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
    // ��ѯ��3�� ���ٸ�ģʽ�������� str �У��ظ���ģʽ��ֻ��һ���� �ظ����ֵ�Ҳֻ��һ��
    // ���ģʽ�� �� abc abc abc def def defg  ��ô�� abcabcdefdefdef ��ͳ�ƴ����� 2��1��abc��1��def
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
                    end[temp] = -end[temp]; //�为��ʾ�Ѿ������
                    stk.push(temp);
                }
                temp = fail[temp];
            }
        }
        //�ָ�ԭ�ȵı�ǣ��Ա��´β�ѯ
        //������ AC�Զ��� ��һ���Եģ��Ͳ���Ҫ�����
        while(!stk.empty()) {
            int temp = stk.top();
            stk.pop();
            end[temp] = -end[temp];
        }
        return res;
    }
    // ��ѯ��4�� str���г����˼���ģʽ�����ظ���ģʽ��ֻ��һ���� �ظ����ֵ�����
    // ���ģʽ�� �� abc abc abc def def defg  ��ô�� abcabcdefdefdef ��ͳ�ƴ����� 5��2��abc��3��def
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
    //��ѯ
    //�ֵ����������ܣ�ͳ�� ǰ׺�� prefix ��ģʽ���ж��ٸ�
    //��������: ����Ҫ build
    int query_prefix(const char* prefix) {
        int now = root;
        for(int i = 0; prefix[i]; i++) {
            int x = prefix[i] - 'a';
            int son = next[now][x];
            if(prefix_cnt[son] == 0)
                return 0; //˵��û��
            else
                now = son;
        }
        return prefix_cnt[now];
    }
    //��ѯ
    //�ֵ����������ܣ�ѯ�� str �Ƿ񱻲��뵽�ֵ����У�����ȫƥ��ĵ���
    //��������: ����Ҫ build��build�� �Ͳ����� ������ȫƥ���ˣ�
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
//�򵥲���
int main() {
    int T;
    int n;
    puts("�����������");
    scanf("%d", &T);
    while(T--) {
        puts("����ģʽ������");
        scanf("%d", &n);//ģʽ������
        trie::init();
        printf("����%d��ģʽ��\n", n);
        for(int i = 0; i < n; i++) {
            scanf("%s", buf);
            trie::insert(buf);
        }
        trie::build();
        puts("�����ѯ��");
        scanf("%s", buf);
        //------------------------------
        puts("ͳ�ƶ��ٸ�ģʽ�������ڲ�ѯ���У�");
        printf("%d\n", ac.query1(buf));
        puts("ͳ�Ʋ�ѯ���г����˼���ģʽ����");
        printf("%d\n", ac.query2(buf));
        puts("ͳ�ƶ��ٸ�ģʽ�������ڲ�ѯ���У��ظ���ģʽ�����㣺");
        printf("%d\n", ac.query3(buf));
        puts("ͳ�Ʋ�ѯ���г����˼���ģʽ�����ظ���ģʽ�����㣺");
        printf("%d\n", ac.query4(buf));
        //------------------------------
        puts("����һ��ǰ׺");
        scanf("%s", buf);
        puts("ͳ�ƶ��ٸ�ģʽ��������ǰ׺��");
        printf("%d\n", ac.query_prefix(buf));
        //------------------------------
        puts("����һ�����ʣ������Ҵ˵����Ƿ���ֹ�");
        scanf("%s", buf);
        printf("%s\n", ac.has(buf) ? "true" : "false");
        //
        
    }
    return 0;
}