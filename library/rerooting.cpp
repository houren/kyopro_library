struct Edge{ int to; };
using G = vector<vector<Edge>>;
template <class S, S (*merge)(S, S), S (*proc)(S), S (*e)()> struct Rerooting{
    vector<S> dp, ans;
    vector<vector<S>> totl, totr;
    G g;
    Rerooting(const G& _g) : g(_g) { init(); }
    void init(){
        int _n = g.size();
        dp = vector<S>(_n);
        ans = vector<S>(_n);
        totl = vector<vector<S>>(_n);
        totr = vector<vector<S>>(_n);
        dfs(-1, 0);
        dfs2(-1, 0, e());
    }
    S dfs(int par, int now){
        S res = e();
        vector<S> list;
        totl[now].emplace_back(res);
        totr[now].emplace_back(res);
        for(auto &edge:g[now]){
            if(par==edge.to) continue;
            list.emplace_back(dfs(now, edge.to));
            res = merge(res, list.back());
            totl[now].emplace_back(res);
        }
        for(int i=list.size()-1;i>=0;i--){
            totr[now].emplace_back(merge(totr[now].back(), list[i]));
        }
        reverse(totr[now].begin(), totr[now].end());
        dp[now] = res;
        return proc(res);
    }
    void dfs2(int par, int now, S k){
        ans[now] = merge(k, dp[now]);
        int i = 0;
        for(auto &edge:g[now]){
            if(par==edge.to) continue;
            dfs2(now, edge.to, proc(merge(merge(k, totl[now][i]), totr[now][i+1])));
            i++;
        }
    }
    vector<S> res(){ return ans; }
};