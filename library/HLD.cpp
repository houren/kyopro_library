// op(S,S): 交換律を満たす二項演算(結合律は未実装)
// Edge: 重みはSであること
// V: Sの配列型 vector<S>を引数に与えると生成されるデータ構造であること
// F: S->Sの関数
// R(V v, int l, int r, F x): 区間[l,r)操作クエリ
// Q(V v, int l, int r): 区間[l,r)演算処理クエリ 返り値の型はSであること
template<class S, S (*op)(S, S), S (*e)(), class Edge, class V, class F, 
         void (*R)(V&, int, int, F), S (*Q)(V&, int, int)>
struct HLD{
    int n, root;
    vector<vector<Edge>> g;
    vector<S> data_;
    V data;
    vector<int> st_size, label, par, top;
    HLD(){}
    HLD(const vector<vector<Edge>>& _g, int r = 0) : 
    n(_g.size()), g(_g), root(r), data_(n), st_size(n,1), label(n), par(n,-1), top(n){
        par[root] = -1;
        dfs_size(root);
        int num = 0;
        dfs_hld(root, num, root);
        data = V(data_);
    }
    void dfs_size(int now){
        for(Edge& edge:g[now]){
            if(par[now]==edge.to) continue;
            par[edge.to] = now;
            dfs_size(edge.to);
            st_size[now] += st_size[edge.to];
            if(st_size[g[now][0].to] < st_size[edge.to]) swap(g[now][0], edge);
        }
    }
    void dfs_hld(int now, int& num, int t){
        label[now] = num, top[now] = t;
        for(Edge& edge:g[now]){
            if(edge.to==par[now]) continue;
            data_[num++] = edge.w;
            dfs_hld(edge.to, num, (edge==g[now][0] ? t : edge.to));
        }
    }
    // {op(u,...,v), lca(u,v)}
    pair<S, int> prod(int u, int v){
        S res = e();
        while(top[u]!=top[v]){
            if(label[u]>label[v]){
                res = op(res, Q(data, label[top[u]] - (top[u]!=root), label[u]));
                u = (top[u]!=root ? par[top[u]] : top[u]);
            }else{
                res = op(res, Q(data, label[top[v]] - (top[v]!=root), label[v]));
                v = (top[v]!=root ? par[top[v]] : top[v]);
            }
        }
        res = op(res, (label[u]<label[v] ? Q(data, label[u], label[v]) : Q(data, label[v], label[u])));
        return {res, (label[u]<label[v] ? u : v)};
    }
    // 列操作クエリ
    int apply(int u, int v, F x){
        if(label[u]<label[v]) swap(u,v);
        while(top[u]!=top[v]){
            R(data, label[top[u]] - (top[u]!=root), label[u], x);
            u = (top[u]!=root ? par[top[u]] : top[u]);
            if(label[u]<label[v]) swap(u,v);
        }
        R(data, label[v], label[u], x);
        return v;
    }
};