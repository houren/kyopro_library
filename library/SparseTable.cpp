template <class S, S (*op)(S, S), S (*e)()>
struct SparseTable{
    vector<vector<S>> data;
    int _n, _log;
    SparseTable(){}
    SparseTable(vector<S> &v){ init(v); }
    void init(vector<S> &v){
        _n = v.size(), _log = 0;
        while((1<<_log)<=_n) _log++;
        data = vector<vector<S>>(_n, vector<S>(_log));
        for(int i=0;i<_n;i++) data[i][0] = v[i];
        for(int j=0;j<_log-1;j++){
            for(int i=0;i+(1<<j)<_n;i++){
                data[i][j+1] = op(data[i][j], data[i+(1<<j)][j]);
            }
        }
    }
    S prod(int l, int r){ //[l,r)
        unsigned int d = 32 - __builtin_clz(r - l) - 1;
        assert(0<=l && l<=r && r<=_n);
        if(l>=r) return e();
        if(l+(1<<d)==r) return data[l][d];
        return op(data[l][d], data[r-(1<<d)][d]);
    }
};
