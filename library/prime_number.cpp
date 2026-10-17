// nまでの素数を列挙します。
// isprime:xが素数であるか判定します。x<=n*n
// pnlist:素数の配列を返します。
struct PrimeNumber{
    vector<int> _PN;
    vector<bool> _B;
    int n;
    PrimeNumber(int _n=0){
        init(_n);
    }
    void init(int _n){
        n = _n;
        _B.assign(n+1,true);
        _B[1] = false;
        for(int i=2;i<=n;i++){
            if(_B[i]){
                _PN.push_back(i);
                for(int j=2*i;j<=n;j+=i) _B[j] = false;
            }
        }
    }
    bool isprime_big(ll x){
        ll m = n;
        assert(x<=m*m);
        for(int &y:_PN){
            if(y*y>x) break;
            if(!(x%y)) return false;
        }
        return true;
    }
    bool isprime(ll x){
        assert(0<x);
        if(n<x) return isprime_big(x);
        return _B[x];
    }
    vector<int> pnlist(){ return _PN; };
};


#include <bits/stdc++.h>
using namespace std;
// #include <atcoder/modint>
// using namespace atcoder;
// using mint = modint998244353;
using ll = long long;
#define fix(x) fixed << setprecision(x)
#define rep(i, n) for(int i = 0; i < n; ++i)
#define all(x) (x).begin(),(x).end()
template<class T>bool chmin(T&a, const T&b){if(a>b){a=b;return 1;}return 0;}
template<class T>bool chmax(T&a, const T&b){if(a<b){a=b;return 1;}return 0;}
constexpr ll INFLL = (1LL << 62), MOD = 998244353;
constexpr int INF = (1 << 30);

#include <boost/multiprecision/cpp_int.hpp>
using namespace boost::multiprecision;
uint64_t modpow(uint64_t n, uint64_t d, uint64_t mod){
    uint128_t res = 1, t = n;
    while(d){
        if(d&1) res = res * t % mod;
        d >>= 1;
        t = t * t % mod;
    }
    return (uint64_t)res;
}
constexpr vector<uint64_t> JUDGE_NUMS = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};
bool is_prime(uint64_t n){
    if(n==2) return 1;
    if(n==1 || !(n&1)) return 0;
    uint64_t m = n - 1, ctz = __builtin_ctzll(n), d = m >> ctz;
    for(uint64_t a:JUDGE_NUMS){
        if(a==n) return 1;
        uint128_t x = modpow(a, d, n);
        if(x==1) continue;
        uint64_t cnt = 0;
        while(x!=m){
            x = x * x % n;
            ++cnt;
            if(x==1 || cnt==ctz) return 0;
        }
    }
    return 1;
}
//n:not prime
uint64_t getfac(uint64_t n){
    if(!(n&1)) return 2;
    uint64_t m = pow(n,0.125) + 1;
    for(uint64_t c=1;c<n;++c){
        uint128_t y = 0, q = 1, r = 1;
        while(1){
            int k = 0;
            uint128_t x = y;
            while(k < ((3*r)>>2)){
                y = (y * y + c) % n;
                ++k;
            }
        }
    }
}
vector<pair<uint64_t,int>> factorize(uint64_t n){
    vector<pair<uint64_t,int>> res;
    if(!(n&1)){
        auto ctz = __builtin_ctzll(n);
        res.emplace_back(2, ctz);
        n >>= ctz;
    }
    while(1){
        if(is_prime(n)){
            res.emplace_back(n,1);
            break;
        }
        uint64_t d = getfac(n);
        int cnt = 0;
        while(!(n%d)) n /= d, ++cnt;
        res.emplace_back(d, cnt);
    }
    return res;
}

int main(){
    cin.tie(nullptr);
    ios::sync_with_stdio(false);
    return 0;
}
