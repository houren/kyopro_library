template <class S, S (*op)(S, S), S (*e)(), S (*inv)(S)>
struct EulerSegtree{
    struct Edge{
        int to, num;
        S s;
    };
    int _n, edge_num = 0, seg_num = 0, log = 0;
    bool _is_edge;
    vector<S> vec;
    segtree<S,op,e> seg;
    vector<vector<Edge>> g;
    vector<int> to, re, in, out, rank;
    vector<vector<int>> anc;
    EulerSegtree(int n, bool is_edge = true): _n(n), _is_edge(is_edge), g(n), to(n-1), re(n-1), in(n), out(n), rank(n){}
    int add_edge(int u, int v, S s){
        g[u].push_back({v,edge_num,s});
        g[v].push_back({u,edge_num,s});
        return edge_num++;
    }
    void dfs(int par, int now, int r){
        in[now] = seg_num;
        rank[now] = r;
        for(auto ed:g[now]){
            if(ed.to==par) continue;
            vec.push_back(ed.s);
            to[ed.num] = seg_num++;
            dfs(now, ed.to, r+1);
            vec.push_back((_is_edge? inv(ed.s): e()));
            re[ed.num] = seg_num++;
            anc[0][ed.to] = now;
        }
        out[now] = seg_num;
    }
    void build(int root = 0){
        int k = 1;
        while(k<_n) k <<= 1, log++;
        anc = vector<vector<int>>(log, vector<int>(_n,-1));
        dfs(-1, root, 0);
        seg = segtree<S,op,e>(vec);
        for(int i=0;i<log-1;i++){
            for(int j=0;j<_n;j++){
                if(anc[i][j]>=0) anc[i+1][j] = anc[i][anc[i][j]];
            }
        }
    }
    int lca(int u, int v){
        if(rank[u]>rank[v]) swap(u,v);
        int k = rank[v] - rank[u];
        for(int i=0;k>0&&i<log;i++){
            if(k&1) v = anc[i][v];
            k >>= 1;
        }
        for(int i=log-1;u!=v&&i>=0;i--){
            if(anc[i][u]!=anc[i][v]){
                u = anc[i][u], v = anc[i][v];
            }
        }
        return (u!=v?anc[0][u]:u);
    }
    S prod(int u, int v){
        assert(_is_edge);
        int x = lca(u, v);
        return op(seg.prod(in[x], out[u]), seg.prod(in[x], out[v]));
    }
    S prod(int v){
        assert(!_is_edge);
        return seg.prod(in[v], out[v]);
    }
    void set(int x, S s){
        seg.set(to[x], s);
        seg.set(re[x], (_is_edge? inv(s): e()));
    }
};
