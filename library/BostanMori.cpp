// [x^k]f/g
mint BostanMori(vector<mint> f, vector<mint> g, ll k){
    vector<mint> g_, s, t;
    int m;
    while(k){
        m = g.size();
        g_ = g;
        for(int i=1;i<m;i+=2) g_[i] = -g_[i];
        s = convolution(f, g_), t = convolution(g, g_);
        f.resize(min<ll>(k+1, (s.size()+1-(k&1))>>1));
        g.resize(min<ll>(k+1, (t.size()+1)>>1));
        for(int i=k&1;i<(int)s.size()&&((i>>1)<=k);i+=2) f[i>>1] = s[i];
        for(int i=0;i<(int)t.size()&&((i>>1)<=k);i+=2) g[i>>1] = t[i];
        k >>= 1;
    }
    return f[0] / g[0];
}