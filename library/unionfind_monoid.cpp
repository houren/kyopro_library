template <class S, S (*op)(S, S), S (*e)()>
struct UnionFind{
    int cnt;
    vector<int> par, siz;
    vector<S> data;
    UnionFind(int _n):UnionFind(vector<S>(_n, e())){}
    UnionFind(const vector<S>& _) : par(_.size()), siz(_.size(), 1), data(_){
        iota(par.begin(), par.end(), 0);
        cnt = _.size();
    }
    int root(int x){
        if(par[x]==x) return x;
        return par[x] = root(par[x]);
    }
    bool unite(int x,int y){
        x = root(x); y = root(y);
        if(x==y) return false;
        --cnt;
        if(siz[x]<siz[y]) swap(x,y);
        par[y] = x;
        siz[x] += siz[y];
        data[x] = op(data[x], data[y]);
        return true;
    }
    void add(int x, S a){
        x = root(x);
        data[x] = op(data[x], a);
    }
    int size(int x){
        return siz[root(x)];
    }
    bool same(int x,int y){
        return root(x) == root(y);
    }
    S val(int x){
        return data[root(x)];
    }
};