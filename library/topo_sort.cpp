struct Edge{ int to; };
using G = vector<vector<Edge>>;
vector<int> topo_sort(G g){
    int n = g.size(), now;
    vector<int> res, cnt(n,0);
    for(int i=0;i<n;i++){
        for(Edge& e:g[i]) cnt[e.to]++;
    }
    stack<int> sta;
    for(int i=n-1;i>=0;i--) if(!cnt[i]) sta.push(i);
    while(sta.size()){
        res.emplace_back(now = sta.top());
        sta.pop();
        for(Edge& e:g[now]){
            if(!(--cnt[e.to])) sta.push(e.to);
        }
    }
    if(res.size()!=n) return vector<int>(1,-1);
    return res;
}