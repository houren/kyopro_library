// primitive_root = 3;
// es[i] = 3^(119 * 2 ^ (23-2-i)), ies[i] = inv(es[i]);
// R[i] = ies[0] * ... * ies[i - 1] * es[i], I[i] = es[0] * ... * es[i - 1] * ies[i];

constexpr ll R[22] = {911660635, 509520358, 369330050, 332049552, 983190778, 123842337, 238493703, 975955924, 603855026, 856644456, 131300601, 842657263, 730768835, 942482514, 806263778, 151565301, 510815449, 503497456, 743006876, 741047443, 56250497, 867605899};
constexpr ll I[22] = {86583718, 372528824, 373294451, 645684063, 112220581, 692852209, 155456985, 797128860, 90816748, 860285882, 927414960, 354738543, 109331171, 293255632, 535113200, 308540755, 121186627, 608385704, 438932459, 359477183, 824071951, 103369235};
ll mpow(ll x, ll y){
    ll res = 1; x %= MOD;
    while(y){
        if(y%2) res = res * x % MOD;
        x = x * x % MOD;
        y /= 2;
    }
    return res;
}
void NTT(vector<ll>& a){
    int n = a.size(), d = __builtin_ffs(n) - 1, l, r;
    for(int i=1;i<=d;++i){
        ll x = 1;
        const int p = 1 << (i-1), q = 1 << (d - i);
        for(int j=0;j<p;++j){
            int offset = j << (d-i+1);
            for(int k=0;k<q;++k){
                l = a[k + offset], r = a[k + offset + q] * x % MOD;
                a[k + offset] = l + r;
                if(a[k+offset]>=MOD) a[k+offset] -= MOD;
                a[k + offset + q] = l - r;
                if(a[k+offset+q]<0) a[k+offset+q] += MOD;
            }
            x = x * R[__builtin_ctz(~(unsigned int)(j))] % MOD;
        }
    }
}
void NTT_inv(vector<ll>& a){
    int n = a.size(), d = __builtin_ffs(n) - 1, l, r;
    for(int i=d;i>=1;--i){
        ll x = 1;
        const int p = 1 << (i-1), q = 1 << (d - i);
        for(int j=0;j<p;++j){
            int offset = j << (d-i+1);
            for(int k=0;k<q;++k){
                l = a[k + offset], r = a[k + offset + q];
                a[k + offset] = l + r;
                if(a[k+offset]>=MOD) a[k+offset] -= MOD;
                a[k + offset + q] = (MOD + l - r) * x % MOD;
            }
            x = x * I[__builtin_ctz(~(unsigned int)(j))] % MOD;
        }
    }
}
vector<ll> convolution(vector<ll> a, vector<ll> b){
    int n = a.size(), m = b.size(), x = 1;
    while(n+m-1>x) x <<= 1;
    a.resize(x); b.resize(x);
    for(int i=n;i<x;++i) a[i] = 0;
    for(int i=m;i<x;++i) b[i] = 0;
    NTT(a); NTT(b);
    for(int i=0;i<x;++i) a[i] = a[i] * b[i] % MOD;
    NTT_inv(a);
    ll y = mpow(x, MOD-2);
    for(int i=0;i<x;++i) a[i] = a[i] * y % MOD;
    a.resize(n+m-1);
    return a;
}
vector<ll> fps_inv(const vector<ll>& f, int n = -1){
    assert(f.size());
    assert(f[0]!=0);
    if(n<0) n = f.size();
    vector<ll> g(1, mpow(f[0], MOD-2));
    int t = 1, d = f.size();
    while(t<n){
        t <<= 1;
        auto g_ = g;
        g = convolution(convolution(vector<ll>(f.begin(), f.begin()+min(t,d)), g), g);
        g.resize(t);
        for(int i=0;i<t;++i) g[i] = (g[i]>0 ? MOD-g[i] : 0);
        for(int i=0;i<(t>>1);++i){
            g[i] = (g[i] + 2*g_[i]) % MOD;
            while(g[i]>=MOD) g[i] -= MOD;
        }
    }
    g.resize(n);
    return g;
}
vector<ll> fps_log(const vector<ll>& f, int n = -1){
    assert(f.size());
    assert(f[0]==1);
    if(n<0) n = f.size();
    int d = f.size();
    auto g = f;
    for(int i=1;i<d;++i) g[i-1] = g[i] * i % MOD;
    g[d-1] = 0;
    g = convolution(g, fps_inv(f, n));
    vector<int> inv(n+2);
    inv[0] = inv[1] = 1;
    for(int i=2;i<n;++i) inv[i] = MOD - inv[MOD%i] * (MOD/i) % MOD;
    for(int i=n-1;i>=1;--i) g[i] = g[i-1] * inv[i] % MOD;
    g[0] = 0;
    return g;
}
vector<ll> fps_exp(const vector<ll>& f, int n = -1){
    assert(f.size());
    assert(!f[0]);
    if(n<0) n = f.size();
    int t = 1, d = f.size();
    vector<ll> g(1,1);
    while(t<n){
        t <<= 1;
        auto g_ = fps_log(g, t);
        for(ll& x:g_) x = (x>0 ? MOD-x : 0);
        for(int i=0;i<min(t,d);++i){
            g_[i] = (g_[i] + f[i]) % MOD;
            if(g_[i]>=MOD) g_[i] -= MOD;
        }
        if(g_[0]!=MOD-1) g_[0]++;
        else g_[0] = 0;
        g = convolution(g, g_);
        g.resize(t);
    }
    g.resize(n);
    return g;
}
vector<ll> fps_pow(vector<ll> f, long long m, int n = -1){
    assert(f.size() && m>=0);
    int d = f.size(), x = 0;
    if(n<0) n = (d-1) * m + 1;
    if(m==0){
        vector<ll> res(n,0);
        res[0] = 1;
        return res;
    }
    while(x<d && f[x]==0) ++x;
    if(x==d || (n+m-1)/m<=x) return vector<ll>(n,0);
    ll y = f[x], z = mpow(y, MOD-2);
    y = mpow(y, m);
    for(int i=x;i<d;++i) f[i] = f[i] * z % MOD;
    f = fps_log(vector<ll>(f.begin()+x, f.end()), n-x*m);
    for(ll& k:f) k = k * (m % MOD) % MOD;
    f = fps_exp(f);
    for(ll& k:f) k = k * y % MOD;
    vector<ll> g(n,0);
    for(int i=0,j=x*m;j<n;++i,++j) g[j] = f[i];
    return g;
}



/*vector<ll> NTT(vector<ll> a){
    int n = a.size(), k = __builtin_ffs(n) - 1;
    if(n==1) return a;
    assert(k<24 && (1<<k)==n);
    vector<ll> even(n/2), odd(n/2);
    for(int i=0;i<n/2;++i){
        even[i] = a[i*2], odd[i] = a[i*2+1];
    }
    even = NTT(even); odd = NTT(odd);
    ll x = 1;
    for(int i=0;i<n/2;++i){
        odd[i] = odd[i] * x % MOD;
        a[i] = (even[i] + odd[i]) % MOD;
        a[n/2+i] = (even[i] - odd[i] + MOD) % MOD;
        x = x * R[k] % MOD;
    }
    return a;
}*/
/*vector<ll> NTT_inv(vector<ll> a){
    int n = a.size(), k = __builtin_ffs(n) - 1;
    if(n==1) return a;
    assert(k<24 && (1<<k)==n);
    vector<ll> even(n/2), odd(n/2);
    for(int i=0;i<n/2;++i){
        even[i] = a[i*2], odd[i] = a[i*2+1];
    }
    even = NTT_inv(even); odd = NTT_inv(odd);
    ll x = 1;
    for(int i=0;i<n/2;++i){
        odd[i] = odd[i] * x;
        a[i] = (even[i] + odd[i]) % MOD;
        a[n/2+i] = ((even[i] - odd[i]) % MOD + MOD) % MOD;
        x = x * I[k] % MOD;
    }
    return a;
}*/
