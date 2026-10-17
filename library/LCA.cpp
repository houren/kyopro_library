struct LCA{
    int _n, _log;
    vector<int> rank;
    vector<vector<int>> d;
    LCA(vector<vector<int>>& g, int root = 0){ init(g, root); }
    void init(vector<vector<int>>& g, int root = 0){
        _n = g.size(), _log = 0;
        while((1<<_log)<=_n) _log++;
        rank = vector<int>(_n);
        rank[root] = 0;
        d = vector<vector<int>>(_n, vector<int>(_log));
        dfs(-1,root,g);
        for(int j=0;j<_log-1;j++){
            for(int i=0;i<_n;i++){
                if(d[i][j]>=0) d[i][j+1] = d[d[i][j]][j];
                else d[i][j+1] = -1;
            }
        }
    }
    void dfs(int par, int now, vector<vector<int>>& g){
        d[now][0] = par;
        for(int x:g[now]){
            if(x==par) continue;
            rank[x] = rank[now]+1;
            dfs(now,x,g);
        }
    }
    int prev(int u, int x){
        assert(x>=0);
        for(int i=0;i<_log;i++){
            if(x&1) u = d[u][i];
            x >>= 1;
            if(!x || u<0) return u;
        }
        return -1;
    }
    int lca(int u, int v){
        if(rank[u]>rank[v]) swap(u,v);
        v = prev(v, rank[v]-rank[u]);
        for(int i=_log-1;i>=0;i--){
            if(d[u][i]!=d[v][i]) u = d[u][i], v = d[v][i];
        }
        if(u!=v) return d[u][0];
        return u;
    }
    int dist(int u, int v){
        return rank[u] + rank[v] - 2*rank[lca(u,v)];
    }
};