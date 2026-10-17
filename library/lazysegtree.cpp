template <class S,
          S (*op)(S, S),
          S (*e)(),
          class F,
          S (*mapping)(F, S),
          F (*composition)(F, F),
          F (*id)()>
struct lazy_segtree{
    int n, size, log;
    vector<S> d;
    vector<F> lz;
    lazy_segtree() : lazy_segtree(0){}
    lazy_segtree(int _n) : lazy_segtree(vector<S>(_n, e())){}
    lazy_segtree(vector<S>& v) : n(int(v.size())){
        while((1<<log) < n) log++;
        size = 1 << log;
        d = vector<S>(2 * size, e());
        lz = vector<F>(size, id());
        for (int i = 0; i < n; i++) d[size + i] = v[i];
        for (int i = size - 1; i >= 1; i--){
            update(i);
        }
    }
    void update(int k){ d[k] = op(d[2 * k], d[2 * k + 1]); }
    void all_apply(int k, F f){
        d[k] = mapping(f, d[k]);
        if (k < size) lz[k] = composition(f, lz[k]);
    }
    void push(int k){
        all_apply(2 * k, lz[k]);
        all_apply(2 * k + 1, lz[k]);
        lz[k] = id();
    }
    void set(int p, S x){
        assert(0 <= p && p < n);
        p += size;
        for (int i = log; i >= 1; i--) push(p >> i);
        d[p] = x;
        for (int i = 1; i <= log; i++) update(p >> i);
    }
    S get(int p){
        assert(0 <= p && p < n);
        p += size;
        for (int i = log; i >= 1; i--) push(p >> i);
        return d[p];
    }
    S prod(int l, int r){
        assert(0 <= l && l <= r && r <= n);
        if (l == r) return e();
        l += size;
        r += size;
        for (int i = log; i >= 1; i--){
            if (((l >> i) << i) != l) push(l >> i);
            if (((r >> i) << i) != r) push(r >> i);
        }
        S sml = e(), smr = e();
        while (l < r){
            if (l & 1) sml = op(sml, d[l++]);
            if (r & 1) smr = op(d[--r], smr);
            l >>= 1;
            r >>= 1;
        }
        return op(sml, smr);
    }
    S all_prod(){ return d[1]; }
    void apply(int p, F f){
        assert(0 <= p && p < n);
        p += size;
        for (int i = log; i >= 1; i--) push(p >> i);
        d[p] = mapping(f, d[p]);
        for (int i = 1; i <= log; i++) update(p >> i);
    }
    void apply(int l, int r, F f){
        assert(0 <= l && l <= r && r <= n);
        if (l == r) return;
        l += size;
        r += size;
        for (int i = log; i >= 1; i--){
            if (((l >> i) << i) != l) push(l >> i);
            if (((r >> i) << i) != r) push((r - 1) >> i);
        }
        int l2 = l, r2 = r;
        while (l < r){
            if (l & 1) all_apply(l++, f);
            if (r & 1) all_apply(--r, f);
            l >>= 1;
            r >>= 1;
        }
        l = l2;
        r = r2;
        for (int i = 1; i <= log; i++){
            if (((l >> i) << i) != l) update(l >> i);
            if (((r >> i) << i) != r) update((r - 1) >> i);
        }
    }
};