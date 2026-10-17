template <typename S>
struct maxflow{
    struct Edge{ int to, rev; S cap; };
    vector<vector<Edge>> g;
    vector<int> level, iter;
    vector<pair<int,int>> edge;
    int n, s, t, m = 0;
    S res = 0;
    maxflow(int n_):n(n_),g(n_),level(n_),iter(n_){}
    int add_edge(int from, int to, ll cap){
        edge.push_back({to, g[to].size()});
        g[from].push_back({to, (int)g[to].size(), cap});
        g[to].push_back({from, (int)g[from].size()-1, 0});
        return m++;
    }
    void bfs(){
        level.assign(n,-1);
        queue<int> que;
        level[s] = 0;
        que.push(s);
        while(que.size()){
            int now = que.front();
            que.pop();
            for(Edge &e:g[now]){
                if(e.cap>0 && level[e.to]<0){
                    level[e.to] = level[now] + 1;
                    que.push(e.to);
                }
            }
        }
    }
    S dfs(int now, S f){
        if(now==t) return f;
        for(int &i=iter[now]; i<g[now].size() ; i++){
            Edge &e = g[now][i];
            if(e.cap>0 && level[now]<level[e.to]){
                int d = dfs(e.to, min(f,e.cap));
                if(d>0){
                    e.cap -= d;
                    g[e.to][e.rev].cap += d;
                    return d;
                }
            }
        }
        return 0;
    }
    S flow(int s_,int t_){
        s = s_, t = t_;
        while(1){
            bfs();
            if(level[t] < 0) break;
            iter.assign(n,0);
            ll f;
            while((f = dfs(s, INF)) > 0) res += f;
        }
        return res;
    }
    S get_edge(int x){
        assert(x < m);
        return g[edge[x].first][edge[x].second].cap;
    }
    vector<S> edges(){
        vector<S> res;
        for(auto& [p,q]:edge) res.emplace_back(g[p][q].cap);
        return res;
    }
};