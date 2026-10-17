struct Mo{
    int n;
    vector<pair<int,int>> lr;
    void add(int l, int r){ //[l,r)
        lr.emplace_back(l,r);
    }
    Mo(int n):n(n){}
    template<typename AL, typename AR, typename EL, typename ER, typename O>
    void build(const AL &al, const AR &ar, const EL &el, const ER &er, const O &out){
        int q = lr.size(), bs = n / min<int>(n, sqrt(q));
        vector<int> ord(q);
        iota(ord.begin(), ord.end(), 0);
        sort(ord.begin(), ord.end(), [&](int a, int b){
            int ablock = lr[a].first / bs, bblock = lr[b].first / bs;
            if(ablock != bblock) return ablock < bblock;
            return (ablock & 1) ? lr[a].second > lr[b].second : lr[a].second < lr[b].second;
        });
        int l = 0, r = 0;
        for(auto idx : ord) {
            while(l > lr[idx].first) al(--l);
            while(r < lr[idx].second) ar(r++);
            while(l < lr[idx].first) el(l++);
            while(r > lr[idx].second) er(--r);
            out(idx);
        }
    }
    template< typename A, typename E, typename O >
    void build(const A &add, const E &erase, const O &out) {
        build(add, add, erase, erase, out);
    }
};