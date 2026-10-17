constexpr int MAX_N = 5050, MOD = 1000000007;
vector<ll> fac(MAX_N), finv(MAX_N), inv(MAX_N);
void init(){
    fac[0] = fac[1] = 1;
    finv[0] = finv[1] = 1;
    inv[1] = 1;
    for(int i=2;i<MAX_N;++i){
        fac[i] = fac[i-1] * i % MOD;
        inv[i] = MOD - inv[MOD%i] * (MOD/i) % MOD;
        finv[i] = finv[i-1] * inv[i] % MOD;
    }
}
ll mcomb(int n,int k){
    if (n<k) return 0;
    if (n<0 || k<0) return 0;
    return fac[n] * (finv[k] * finv[n-k] % MOD) % MOD;
}
ll mperm(int n,int k){
    if (n<k) return 0;
    if (n<0 || k<0) return 0;
    return fac[n] * finv[n-k] % MOD;
}