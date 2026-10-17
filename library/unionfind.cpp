struct UnionFind{
    int cnt;
    vector<int> par, siz;
    UnionFind(int N) : par(N), siz(N,1){
        rep(i,N) par[i] = i;
        cnt = N;
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
        return true;
    }
    int size(int x){
        return siz[root(x)];
    }
    bool same(int x,int y){
        return root(x) == root(y);
    }
};