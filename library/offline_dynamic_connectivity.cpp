template <class S, S (*op)(S, S), S (*e)()>
struct UndoableUnionFind{
    int num;
    vector<int> data;
    vector<S> val;
    stack<tuple<int,int,S>> hist;
    UndoableUnionFind() = default;
    explicit UndoableUnionFind(int N) : num(N), data(N, -1), val(N, e()){}
    explicit UndoableUnionFind(const vector<S>& v) : num(v.size()), data(num, -1), val(v){}
    int root(int x){ return data[x] < 0 ? x : root(data[x]); }
    bool unite(int x,int y){
        x = root(x); y = root(y);
        hist.emplace(-1, num, e());
        hist.emplace(x, data[x], val[x]);
        hist.emplace(y, data[y], val[y]);
        if(x==y) return false;
        if(data[x] > data[y]) swap(x, y);
        data[x] += data[y];
        data[y] = x;
        val[x] = op(val[x], val[y]);
        return true;
    }
    int size(int x){
        return -data[root(x)];
    }
    bool same(int x,int y){
        return root(x) == root(y);
    }
    bool undo(){
        if(!hist.size()) return false;
        while(1){
            auto [x, y, z] = hist.top();
            hist.pop();
            if(x < 0){
                num = y;
                break;
            }
            data[x] = y, val[x] = z;
        }
        return true;
    }
    S prod(int x){ return val[root(x)]; }
    void update(int x, const S& v){
        x = root(x);
        hist.emplace(-1, num, e());
        hist.emplace(x, data[x], val[x]);
        val[x] = op(val[x], v);
    }
};

template <class S, S (*op)(S, S), S (*e)(), class T = int>
struct OfflineDynamicConnectivity{
    int time = 0;
    bool exist_e = true;
    map<pair<int,int>, pair<int,T>> ct_e;
    map<pair<int,int>, T> lz_e;
    vector<tuple<int,int,int,int>> e_rng; // u, v, [l, r)
    vector<pair<int, int>> query;
    vector<tuple<int, S, int>> q_upd; // v, x, l
    vector<int> uq;
    vector<S> val;

    OfflineDynamicConnectivity() = default;
    explicit OfflineDynamicConnectivity(int N) : val(N, e()){}
    explicit OfflineDynamicConnectivity(const vector<S>& v) : val(v){}
    // 必要があるときに操作を走査する
    void trv_e(){
        if(!exist_e) return;
        exist_e = false;
        for(auto& [p,q]:lz_e){
            if(!ct_e.count(p)){
                if(q>0) ct_e[p] = {time, q};
            }else{
                auto& x = ct_e[p];
                x.second += q;
                if(x.second<=0){
                    e_rng.emplace_back(p.first, p.second, x.first, time);
                    ct_e.erase(p);
                }
            }
        }
        lz_e.clear();
    }
    void trv_query(){
        if(exist_e) return;
        exist_e = true;
        uq.emplace_back(query.size());
        ++time;
    }
    void edge_sub(int u, int v, T x){
        if(!x) return;
        trv_query();
        lz_e[minmax(u,v)] += x;
    }
    void add_edge(int u, int v){ edge_sub(u, v, 1); }
    void erase_edge(int u, int v){ edge_sub(u, v, -1); }
    void update(int v, S x){
        if(x==e()) return;
        trv_query();
        q_upd.emplace_back(v, x, time);
    }

    // same:(u,v) size:(-1,v) num:(-1,-1) prod:(-2,v)
    void same(int u, int v){
        trv_e();
        query.emplace_back(u, v);
    }
    void size(int v){
        trv_e();
        query.emplace_back(-1, v);
    }
    void num(){
        trv_e();
        query.emplace_back(-1, -1);
    }
    void prod(int v){
        trv_e();
        query.emplace_back(-2, v);
    }

    // first:same,size,num second:prod
    vector<pair<int, S>> run(){
        ++time;
        uq.emplace_back(query.size());
        for(auto& [p,q]:ct_e) e_rng.emplace_back(p.first, p.second, q.first, time);
        int size = bit_ceil((unsigned int)time);
        vector<vector<pair<int, int>>> edges(2*size);
        vector<vector<pair<int, S>>> updates(2*size);
        for(auto [u,v,l,r]:e_rng){
            for(l+=size,r+=size;l<r;l>>=1,r>>=1){
                if(l&1) edges[l++].emplace_back(u,v);
                if(r&1) edges[--r].emplace_back(u,v);
            }
        }
        for(auto [v,x,l]:q_upd){
            int r = size;
            for(l+=size,r+=size;l<r;l>>=1,r>>=1){
                if(l&1) updates[l++].emplace_back(v,x);
                if(r&1) updates[--r].emplace_back(v,x);
            }
        }
        UndoableUnionFind<S, op, e> uf(val);
        vector<pair<int, S>> res;
        int now = 0;
        auto dfs = [&](const auto& self, int k) -> void {
            for(auto [u,v]:edges[k]){ uf.unite(u,v); }
            for(auto [v,x]:updates[k]){ uf.update(v,x); }
            if(k < size) {
                self(self, 2*k);
                self(self, 2*k+1);
            }else{
                int m = k - size;
                if(m < (int)uq.size()){
                    for(;now<uq[m];++now){
                        auto [u,v] = query[now];
                        if(u==-1) res.emplace_back(v<0?uf.num:uf.size(v), e());
                        else if(u==-2) res.emplace_back(-1, uf.prod(v));
                        else res.emplace_back(uf.same(u,v), e());
                    }
                }
            }
            int m = edges[k].size() + updates[k].size() + 1;
            while(--m) uf.undo();
        };
        dfs(dfs, 1);
        return res;
    }
};