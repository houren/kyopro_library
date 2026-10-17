
template <typename Cap, typename Cost>
struct mincostflow{
    int n, m = 0;
    struct Edge{int to, rev; Cap cap; Cost cost;};
    vector<vector<Edge>> g;
    vector<pair<int,int>> edge;
    mincostflow(int _n) : n(_n), g(n) {}
    int add_edge(int from, int to, Cap cap, Cost cost){
        assert(0 <= from && from < n && 0 <= to && to < n && 0 <= cap && 0 <= cost);
        g[from].emplace_back(to, g[to].size(), cap, cost);
        g[to].emplace_back(to, g[to].size()-1, 0, -cost);
        return ++m;
    }
    Cost flow(int s, int t, Cap flow){
        Cost res = 0;
        using P = pair<Cost, int>;
        priority_queue<P, vector<P>, greater<P>> pq;
        vector<Cost> cost, p(n,0);
        while(flow){
            
        }
    }
};
