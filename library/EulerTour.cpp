struct Edge{ int to; };
auto G = vector<vector<Edge>>;

struct EulerTourTree{
    int t = 0;
    vector<int> in, out, res;
    EulerTourTree(G &g):{ init(g); }
    void dfs(int par, int now, G &g){
        res[t] = now;
        in[now] = t++;
        for(auto &e:g[now]){
            if(e.to==par) continue;
            dfs(now, e.to, g);
        }
        res[t] = now;
        out[now] = t++;
    }
    void init(G &g){
        int n = g.size();
        in.resize(n);
        out.resize(n);
        res.resize(2*n);
        dfs(-1, 0, g);
    }
};