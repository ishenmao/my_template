#include <iostream>
#include <string>
#include <vector>

class KMP {
private:
	std::string pattern; //模式串
	//前缀数组，pai[i] := 长度为k的前缀等于长度为k的后缀（以i结尾）的最大k，
	//需要是真前缀，单个字符的字符串的真前缀、真后缀都是空串，所以pai函数都是0
	std::vector<int> pai; //长度等于模式串长度
	void init_pai() {
		if (pattern.empty()) {
			return;
		}
		int m = pattern.length();
		pai.resize(m);
		pai[0] = 0; //初始只有1个字符，所以是0
		/**
		 * if p[i] == p[pai[i-1]]:
		 * 		so pai[i] = pai[i-1] + 1
		 * if p[i] == p[pai[pai[i-1]-1]]:
		 * 		so pai[i] = p[pai[pai[i-1]-1]]
		 * 直到 pai[pai[...]-1] = 0
		 * if p[i] == p[0]:
		 * 		so pai[i] = 1
		 * else: pai[i] = 0
		 */
		for (int i = 1; i < m; i++) {
			int pai_v = pai[i - 1];
			while (pai_v > 0 and pattern[i] != pattern[pai_v]) {
				pai_v = pai[pai_v - 1];
			}
			pai[i] = pai_v + (pattern[i] == pattern[pai_v]);
		}
	}
public:
	KMP() {}
	KMP(const std::string& s) : pattern(s) {
		init_pai();
	}
	void reset_pattern(const std::string& s) {
		pattern = s;
		init_pai();
	}
	//查询第一次匹配的位置，无法匹配返回-1
	int query_pos(const std::string& txt) {
		if (pattern.empty()) {
			std::cerr << "模式串未设置" << std::endl;
			return -1;
		}
		int n = txt.length();
		int m = pattern.length();
		if (n < m) {
			return -1;
		}
		int j = 0;
		for (int i = 0; i < n; i++) {
			while (j > 0 and txt[i] != pattern[j]) {
				j = pai[j - 1];
			}
			if(txt[i] == pattern[j]){
				j++;
			}
			if(j == m){
				//此时已经完成1次匹配
				return i-m+1; //返回txt匹配的首个字符位置
			}
		}
		return -1;
	}
	//查询一共匹配的次数，无法匹配返回 0
	int query_count(const std::string& txt) {
		if (pattern.empty()) {
			std::cerr << "模式串未设置" << std::endl;
			return 0;
		}
		int n = txt.length();
		int m = pattern.length();
		if (n < m) {
			return 0;
		}
		int j = 0, ans = 0;
		for (int i = 0; i < n; i++) {
			while (j > 0 and txt[i] != pattern[j]) {
				j = pai[j - 1];
			}
			if(txt[i] == pattern[j]){
				j++;
			}
			if(j == m){
				//此时已经完成1次匹配
				ans++;
				j = pai[j-1];
			}
		}
		return ans;
	}
};

int main(){
	KMP kmp("abcabd");
	std::cout << kmp.query_pos("abcabcabd");
}
