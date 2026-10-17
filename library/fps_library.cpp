#include <atcoder/convolution>
using namespace atcoder;
using mint = modint998244353;
vector<mint> fps_inv(const vector<mint>& f, int n = -1){
    assert(f.size());
    assert(f[0].val()!=0);
    if(n<0) n = f.size();
    vector<mint> g(1,f[0].inv());
    int t = 1, d = f.size();
    while(t<n){
        t <<= 1;
        auto g_ = g;
        g = convolution(convolution(vector<mint>(f.begin(), f.begin()+min(t,d)), g), g);
        g.resize(t);
        for(int i=0;i<t;++i) g[i] = -g[i];
        for(int i=0;i<(t>>1);++i) g[i] += 2*g_[i];
    }
    g.resize(n);
    return g;
}
pair<vector<mint>,vector<mint>> fps_div(vector<mint> f, vector<mint> g){
    while(f.size()>0 && !f.back().val()) f.pop_back();
    while(g.size()>0 && !g.back().val()) g.pop_back();
    assert(g.size()>0);
    int n = f.size(), m = g.size();
    if(m>n){
        return {vector<mint>(0),f};
    }
    vector<mint> fr = f, gr = g;
    reverse(fr.begin(), fr.end());
    reverse(gr.begin(), gr.end());
    vector<mint> q = convolution(fr,fps_inv(gr,n-m+1));
    q.resize(n-m+1);
    reverse(q.begin(), q.end());
    while(q.size()>0 && !q.back().val()) q.pop_back();
    vector<mint> gq = convolution(g,q), r(m-1,0);
    for(int i=0;i<m-1;++i) r[i] = f[i] - gq[i];
    while(r.size()>0 && !r.back().val()) r.pop_back();
    return {q,r};
}
vector<mint> fps_log(const vector<mint>& f, int n = -1){
    assert(f.size());
    assert(f[0].val()==1);
    if(n<0) n = f.size();
    int d = f.size();
    auto g = f;
    for(int i=1;i<d;++i) g[i-1] = g[i] * i;
    g[d-1] = 0;
    g = convolution(g, fps_inv(f, n));
    vector<mint> inv(n+2);
    inv[0] = inv[1] = 1;
    int mod = mint::mod();
    for(int i=2;i<n;++i) inv[i] = mod - inv[mod%i] * (mod/i);
    for(int i=n-1;i>=1;--i) g[i] = g[i-1] * inv[i];
    g[0] = 0;
    return g;
}
vector<mint> fps_exp(const vector<mint>& f, int n = -1){
    assert(f.size());
    assert(!f[0].val());
    if(n<0) n = f.size();
    int t = 1, d = f.size();
    vector<mint> g(1,1);
    while(t<n){
        t <<= 1;
        auto g_ = fps_log(g, t);
        for(mint& x:g_) x = -x;
        for(int i=0;i<min(t,d);++i) g_[i] += f[i];
        g_[0] += 1;
        g = convolution(g, g_);
        g.resize(t);
    }
    g.resize(n);
    return g;
}
vector<mint> fps_pow(vector<mint> f, long long m, int n = -1){
    assert(f.size() && m>=0);
    int d = f.size(), x = 0;
    if(n<0) n = (d-1) * m + 1;
    if(m==0){
        vector<mint> res(n,0);
        res[0] = 1;
        return res;
    }
    while(x<d && f[x].val()==0) ++x;
    if(x==d || (n+m-1)/m<=x) return vector<mint>(n,0);
    mint y = f[x], z = y.inv();
    y = y.pow(m);
    for(int i=x;i<d;++i) f[i] *= z;
    f = fps_log(vector<mint>(f.begin()+x, f.end()), n-x*m);
    for(mint& k:f) k *= m;
    f = fps_exp(f);
    for(mint& k:f) k *= y;
    vector<mint> g(n,0);
    for(int i=0,j=x*m;j<n;++i,++j) g[j] = f[i];
    return g;
}
vector<mint> multipoint_evaluation(vector<mint> c, vector<mint> p){
    int n = 1, log = 0;
    while(n<(int)p.size()) n <<= 1, ++log;
    vector<vector<mint>> f(2*n);
    for(int i=0;i<(int)p.size();++i) f[n+i] = {-p[i], 1};
    for(int i=n-1;i>=1;--i){
        if(f[2*i+1].size()<1) f[i] = f[2*i];
        else f[i] = convolution(f[2*i], f[2*i+1]);
    }
    f[1] = fps_div(c, f[1]).second;
    for(int i=1;i<n;++i){
        if(f[2*i].size()>0) f[2*i] = fps_div(f[i], f[2*i]).second;
        if(f[2*i+1].size()>0) f[2*i+1] = fps_div(f[i], f[2*i+1]).second;
    }
    vector<mint> res(p.size());
    for(int i=0;i<(int)p.size();++i){
        res[i] = (f[n+i].size()>0?f[n+i][0]:0);
    }
    return res;
}