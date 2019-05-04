#include <bits/stdc++.h>
using namespace std;
typedef long long LL;

// 素数筛, notprime 是素数的标记数组，请保证数组已经初始化好(全false)，n 是范围
// 不筛 2。
// 复杂度线性，n=1e7时，耗时约0.08秒
bool flag[100000006];
int prime[40000007], cnt = 0;
inline void prime_sieve(int n) {
    prime[cnt++] = 2;
    for(int i = 3; i <= n; i += 2) {
        if(!flag[i]) { //如果是素数，加入到prime中
            prime[cnt++] = i;
        }
        for(int k = 1; k < cnt; k++) {
            if(i > n / prime[k])
                break;
            flag[i * prime[k]] = true;
            if(i % prime[k] == 0)
                break; //保证每个数都被标记一次
        }
    }
}

// 大区间素数筛选，给出 L、R，找出 [L, R] 内的素数(埃氏筛)
// 一般 在 10^12 范围，但是 区间长度比较小，通常 10^6
// 需要提前准备好 1 - 10^6 的素数表，并保存在 int prime[] 中
// 返回 prime_big[] 实际大小，当 r < 1000000 时不处理，请直接使用 prime[]
// 复杂度，大约 线性
LL prime_big[1000006];
bool flag_big[1000006];
inline int prime_sieve_big(LL l, LL r) {
    if(r < 1000000)
        return -1;
    memset(big_prime, 0, sizeof(LL) * (r - l + 1));
    for(int i = 0; prime[i] <= r / prime[i]; i++) {
        // 确定起点, j 是 [l, r] 中 第一个 prime[i]的倍数
        LL j = max((LL)prime[i] * prime[i], (l - 1) / prime[i] * prime[i] + prime[i]);
        for(; j <= r; j += prime[i]) {
            flag_big[j - l] = true;
        }
    }
    int cnt = 0;
    int maxn = r - l;
    // 只从奇数开始找
    for(int i = (l & 1 ? 0 : 1); i <= maxn; i += 2) {
        if(!flag_big[i])
            big_prime[cnt++] = l + i;
    }
    return cnt;
}

// 单个数合数分解(1): 保存 x 的质因数及次数，返回质因子个数，即 factor[] 有效大小。
// factor[i] 、expo[i]：表示 x 的一个质因数为 factor[i]，次数是 expo[i], 不需要提前初始化
// 请提前使用素数筛筛出素数，并保存在 int prime[] 的数组中，请确保筛的素数够用
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
// 单个数合数分解(2): 保存 第i个质因数的次数，返回 x 最大素数因子的下标+1。即 expo[] 有效大小
// x 不要超过素数筛的筛选范围，否则 x 或者 x 的某个因子是大素数时出错
// expo[i]：表示第 i 个 质数 次数为 expo[i]
// 请提前使用素数筛筛出素数表，并保存在 int prime[] 的数组中，请确保筛的素数够用
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
// 单个数合数分解(3): 与(1)相同，但不需要素数表
// 复杂度比(1)高一点点，O(sqrt(x)), 所以 x 最大能撑到 1e12
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
// 枚举 n 的所有因子
// 复杂度 O(sqrt(n))
// fact[] 无序
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

// 积性函数，单个素数定义求法
// 积性函数的和函数还是积性函数
// 参数 p 是素数，k是p的指数, type 类型
// 1 : 欧拉函数，2：因子个数（因子0次方和）
// 3: 因子和（因子1次方和）, 4：因子平方和（因子2次方和）
inline int prod_f(int p, int k, int type) {
    auto power = [](int x, int n) { // lambda函数，计算幂，根据需要是否取模，或者改 longlong
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
    else if(type == 2) // 0次方
        return k + 1;
    else if(type == 3) // 1 次方
        return (power(p, k + 1) - 1) / (p - 1); // 根据需要改成 逆元、求模等其他
    else if(type == 4) // 2 次方
        return (power(p * p, k + 1) - 1) / (p * p - 1); // 根据需要改成 逆元、求模等其他
    else { // 其他，自定义的积性函数
        return 0;
    }
    return 0;
}
// 积性函数的和函数(没有加模数)
// prod_sum(n) = sigma[d | n](f(d))
inline int prod_sum(int n) {
    int ans = 1;
    for(int i = 0, p = prime[0]; p <= n / p; i++, p = prime[i]) {
        int k = 0;
        if(n % p == 0) {
            while(n % p == 0)
                n /= p, k++;
            ans *= prod_f(p, k, type); // 根据需要设置 type
        }
    }
    if(n  > 1)
        ans *= prod_f(n, 1, type);
    return ans;
}
// 单个数欧拉函数：给定正整数n，求 [1, n] 内与n互质的数的个数
// 公式 phi(n) = n * (p0-1)/p0 * (p1-1)/p1 * ... * (pk-1)/pk
// 复杂度：sqrt(n)
inline int phi(int n) {
    int ans = 0;
    for(int i = 2; i <= n / i; i++) {
        if(n % i == 0) { // 此时的 i 一定是n的素数因子
            ans = ans / i * (i - 1);
            while(n % i == 0)
                n /= i;
        }
    }
    if(n > 1) // 此时剩下的n是个素数
        ans = ans / n * (n - 1);
    return ans;
}
// 欧拉函数线性筛：求 [1, n] 内所有数的欧拉函数，顺便筛素数
// 当素数 p 满足 p | i，则 phi(i*p) = phi(i)*p
// 当素数 p 满足 p ~| i, 即 i 不是 p 的倍数，那么 i、p 互质，则 phi(i*p) = phi(i)*(p-1)
// 复杂度：O(n)
int phi_arr[1000006] = {0, 1};
bool flag[1000006]; // 素数标记
int prime[1000006], cnt = 0; // 素数表
inline void phi_sieve(int n) {
    for(int i = 2; i <= n; i++) {
        if(!flag[i]) {
            prime[cnt++] = i;
            phi_arr[i] = i - 1;
        }
        for(int k = 0, lim = n / i; k < cnt && prime[k] <= lim; k++) {
            int p = prime[k];
            flag[i * p] = true;
            bool nz = i % p; //非0
            phi_arr[i * p] = phi_arr[i] * (nz ? p - 1 : p);
            if(!nz)
                break; //保证每个数都被标记一次
        }
    }
}