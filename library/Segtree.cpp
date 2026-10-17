template <class S, S (*op)(S, S), S (*e)()>
struct segtree{
    int n, log = 0, size;
    vector<S> d;
    segtree():segtree(0){}
    segtree(int _n):segtree(vector<S>(_n,e())){}
    segtree(vector<S>& v): n(v.size()){
        while((1<<log) < n) log++;
        size = 1 << log;
        d = vector<S>(2*size, e());
        for(int i=0;i<n;i++) d[size+i] = v[i];
        for(int i=size-1;i>=1;i--) update(i);
    }
    void update(int k){ d[k] = op(d[2*k], d[2*k+1]); }
    void set(int p, S x){
        assert(0 <= p && p < n);
        p += size;
        d[p] = x;
        for(int i=1;i<=log;i++) update(p >> i);
    }
    S get(int p){
        assert(0<=p && p<n);
        return d[p+size];
    }
    S prod(int l, int r){
        assert(0 <= l && l <= r && r <= n);
        S sml = e(), smr = e();
        l += size;
        r += size;
        while (l < r) {
            if (l & 1) sml = op(sml, d[l++]);
            if (r & 1) smr = op(d[--r], smr);
            l >>= 1;
            r >>= 1;
        }
        return op(sml, smr);
    }
    S all_prod() { return d[1]; }
};
