template <class S>
struct BipartiteMinCover{
    public:
        BipartiteMinCover():BipartiteMinCover(0){}
        BipartiteMinCover(int n):BipartiteMinCover(vector<S>(n,1)){}
        BipartiteMinCover(const vector<S>& v){
            n = int(v.size());
            w = v;
            g.resize(n);
            for(int i=0;i<n;++i) INF_W += v[i];
        }
        void add_edge(int u, int v){
            assert(0<=u && u<n);
            assert(0<=v && v<n);
            g[u].emplace_back(v);
            g[v].emplace_back(u);
            ++m;
        }
        pair<S,vector<bool>> solve(){
            vector<int> l(n,-1);
            for(int i=0;i<n;++i){
                if(l[i]<0){
                    queue<int> que;
                    que.emplace(i);
                    l[i] = 0;
                    while(que.size()){
                        int now = que.front();
                        que.pop();
                        for(int x:g[now]){
                            if(l[x]<0){
                                l[x] = !l[now];
                                que.emplace(x);
                            }
                            assert(l[x]^l[now]);
                        }
                    }
                }
            }
            mf_graph<S> mg(n+2);
            for(int i=0;i<n;++i){
                if(l[i]<1) mg.add_edge(n,i,w[i]);
                else mg.add_edge(i,n+1,w[i]);
            }
            for(int i=0;i<n;++i){
                if(l[i]>0) continue;
                for(int x:g[i]){
                    mg.add_edge(i,x,INF_W);
                }
            }
            mg.flow(n,n+1);
            auto mc = mg.min_cut(n);
            S val = 0;
            vector<bool> res(n);
            for(int i=0;i<n;++i){
                res[i] = (mc[i]==l[i]);
                val += res[i]*w[i];
            }
            return make_pair(val,res);
        }
    private:
        int n, m = 0;
        S INF_W = 1;
        vector<vector<int>> g;
        vector<S> w;
};
