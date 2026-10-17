ll mpow(ll x, ll y){
    ll res = 1; x %= MOD;
    while(y){
        if(y%2) res = res * x % MOD;
        x = x * x % MOD;
        y /= 2;
    }
    return res;
}