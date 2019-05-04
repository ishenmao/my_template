#include <bits/stdc++.h>
using namespace std;
typedef long long LL;

// ����ɸ, notprime �������ı�����飬�뱣֤�����Ѿ���ʼ����(ȫfalse)��n �Ƿ�Χ
// ��ɸ 2��
// ���Ӷ����ԣ�n=1e7ʱ����ʱԼ0.08��
bool flag[100000006];
int prime[40000007], cnt = 0;
inline void prime_sieve(int n) {
    prime[cnt++] = 2;
    for(int i = 3; i <= n; i += 2) {
        if(!flag[i]) { //��������������뵽prime��
            prime[cnt++] = i;
        }
        for(int k = 1; k < cnt; k++) {
            if(i > n / prime[k])
                break;
            flag[i * prime[k]] = true;
            if(i % prime[k] == 0)
                break; //��֤ÿ�����������һ��
        }
    }
}

// ����������ɸѡ������ L��R���ҳ� [L, R] �ڵ�����(����ɸ)
// һ�� �� 10^12 ��Χ������ ���䳤�ȱȽ�С��ͨ�� 10^6
// ��Ҫ��ǰ׼���� 1 - 10^6 ���������������� int prime[] ��
// ���� prime_big[] ʵ�ʴ�С���� r < 1000000 ʱ��������ֱ��ʹ�� prime[]
// ���Ӷȣ���Լ ����
LL prime_big[1000006];
bool flag_big[1000006];
inline int prime_sieve_big(LL l, LL r) {
    if(r < 1000000)
        return -1;
    memset(big_prime, 0, sizeof(LL) * (r - l + 1));
    for(int i = 0; prime[i] <= r / prime[i]; i++) {
        // ȷ�����, j �� [l, r] �� ��һ�� prime[i]�ı���
        LL j = max((LL)prime[i] * prime[i], (l - 1) / prime[i] * prime[i] + prime[i]);
        for(; j <= r; j += prime[i]) {
            flag_big[j - l] = true;
        }
    }
    int cnt = 0;
    int maxn = r - l;
    // ֻ��������ʼ��
    for(int i = (l & 1 ? 0 : 1); i <= maxn; i += 2) {
        if(!flag_big[i])
            big_prime[cnt++] = l + i;
    }
    return cnt;
}

// �����������ֽ�(1): ���� x �������������������������Ӹ������� factor[] ��Ч��С��
// factor[i] ��expo[i]����ʾ x ��һ��������Ϊ factor[i]�������� expo[i], ����Ҫ��ǰ��ʼ��
// ����ǰʹ������ɸɸ���������������� int prime[] �������У���ȷ��ɸ����������
inline int get_factor(LL factor[], int expo[], LL x) {
    int cnt = 0;
    for(int i = 0; prime[i] <= x / prime[i]; i++) {
        if(x % prime[i] == 0) {
            expo[cnt] = 1;
            factor[cnt] = prime[i];
            x /= prime[i];
            while(x % prime[i] == 0) {
                expo[cnt]++;
                x /= prime[i];
            }
            ++cnt;
        }
    }
    if(x  > 1) {
        factor[cnt] = x;
        expo[cnt++] = 1;
    }
    return cnt;
}
// �����������ֽ�(2): ���� ��i���������Ĵ��������� x ����������ӵ��±�+1���� expo[] ��Ч��С
// x ��Ҫ��������ɸ��ɸѡ��Χ������ x ���� x ��ĳ�������Ǵ�����ʱ����
// expo[i]����ʾ�� i �� ���� ����Ϊ expo[i]
// ����ǰʹ������ɸɸ���������������� int prime[] �������У���ȷ��ɸ����������
inline int get_factor2(int expo[], int x) {
    int cnt = 0;
    for(int i = 0; prime[i] <= x / prime[i]; i++) {
        expo[i] = 0;
        while(x % prime[i] == 0) {
            expo[i]++;
            x /= prime[i];
        }
        if(expo[i])
            cnt = i;
    }
    return cnt + 1;
}
// �����������ֽ�(3): ��(1)��ͬ��������Ҫ������
// ���Ӷȱ�(1)��һ��㣬O(sqrt(x)), ���� x ����ܳŵ� 1e12
inline int get_factor3(LL factor[], int expo[], LL x) {
    int cnt = 0;
    for(int i = 2; i <= x / i; i++) {
        if(x % i == 0) {
            expo[cnt] = 1;
            factor[cnt] = i;
            x /= i;
            while(x % i == 0) {
                expo[cnt]++, x /= i;
            }
            ++cnt;
        }
    }
    if(x  > 1) {
        factor[cnt] = x;
        expo[cnt++] = 1;
    }
    return cnt;
}
// ö�� n ����������
// ���Ӷ� O(sqrt(n))
// fact[] ����
int fact[1600], cnt = 0;
inline void enum_factor(int n) {
    fact[cnt++] = 1;
    int i = 2;
    for(; i < n / i; i++) {
        if(n % i == 0)
            fact[cnt++] = i, fact[cnt++] = n / i;
    }
    if(i * i == n)
        fact[cnt++] = i;
}

// ���Ժ�������������������
// ���Ժ����ĺͺ������ǻ��Ժ���
// ���� p ��������k��p��ָ��, type ����
// 1 : ŷ��������2�����Ӹ���������0�η��ͣ�
// 3: ���Ӻͣ�����1�η��ͣ�, 4������ƽ���ͣ�����2�η��ͣ�
inline int prod_f(int p, int k, int type) {
    auto power = [](int x, int n) { // lambda�����������ݣ�������Ҫ�Ƿ�ȡģ�����߸� longlong
        int ret = 1;
        while(n) {
            if(n & 1)
                ret *= x;
            x *= x, n >>= 1;
        }
        return ret;
    };
    if(type == 1)
        return power(p, k - 1) * (p - 1);
    else if(type == 2) // 0�η�
        return k + 1;
    else if(type == 3) // 1 �η�
        return (power(p, k + 1) - 1) / (p - 1); // ������Ҫ�ĳ� ��Ԫ����ģ������
    else if(type == 4) // 2 �η�
        return (power(p * p, k + 1) - 1) / (p * p - 1); // ������Ҫ�ĳ� ��Ԫ����ģ������
    else { // �������Զ���Ļ��Ժ���
        return 0;
    }
    return 0;
}
// ���Ժ����ĺͺ���(û�м�ģ��)
// prod_sum(n) = sigma[d | n](f(d))
inline int prod_sum(int n) {
    int ans = 1;
    for(int i = 0, p = prime[0]; p <= n / p; i++, p = prime[i]) {
        int k = 0;
        if(n % p == 0) {
            while(n % p == 0)
                n /= p, k++;
            ans *= prod_f(p, k, type); // ������Ҫ���� type
        }
    }
    if(n  > 1)
        ans *= prod_f(n, 1, type);
    return ans;
}
// ������ŷ������������������n���� [1, n] ����n���ʵ����ĸ���
// ��ʽ phi(n) = n * (p0-1)/p0 * (p1-1)/p1 * ... * (pk-1)/pk
// ���Ӷȣ�sqrt(n)
inline int phi(int n) {
    int ans = 0;
    for(int i = 2; i <= n / i; i++) {
        if(n % i == 0) { // ��ʱ�� i һ����n����������
            ans = ans / i * (i - 1);
            while(n % i == 0)
                n /= i;
        }
    }
    if(n > 1) // ��ʱʣ�µ�n�Ǹ�����
        ans = ans / n * (n - 1);
    return ans;
}
// ŷ����������ɸ���� [1, n] ����������ŷ��������˳��ɸ����
// ������ p ���� p | i���� phi(i*p) = phi(i)*p
// ������ p ���� p ~| i, �� i ���� p �ı�������ô i��p ���ʣ��� phi(i*p) = phi(i)*(p-1)
// ���Ӷȣ�O(n)
int phi_arr[1000006] = {0, 1};
bool flag[1000006]; // �������
int prime[1000006], cnt = 0; // ������
inline void phi_sieve(int n) {
    for(int i = 2; i <= n; i++) {
        if(!flag[i]) {
            prime[cnt++] = i;
            phi_arr[i] = i - 1;
        }
        for(int k = 0, lim = n / i; k < cnt && prime[k] <= lim; k++) {
            int p = prime[k];
            flag[i * p] = true;
            bool nz = i % p; //��0
            phi_arr[i * p] = phi_arr[i] * (nz ? p - 1 : p);
            if(!nz)
                break; //��֤ÿ�����������һ��
        }
    }
}