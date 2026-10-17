template <class S, S (*op)(S, S)>
struct SparseTable2D{
    int _n, _m, _logn, _logm;
    vector<vector<vector<vector<S>>>> data;
    SparseTable2D(){}
    SparseTable2D(vector<vector<S>> &v){ init(v); }
    void init(vector<vector<S>> &v){
        _n = v.size(), _m = v[0].size(), _logn = 0, _logm = 0;
        while((1<<_logn)<=_n) _logn++;
        while((1<<_logm)<=_m) _logm++;
        data = vector(_n, vector(_m, vector(_logn, vector<S>(_logm))));

        for(int i=0;i<_n;i++) for(int j=0;j<_m;j++) data[i][j][0][0] = v[i][j];

        for(int k=0;k<_logn-1;k++){
            for(int i=0;i+(1<<k)<_n;i++) for(int j=0;j<_m;j++){
                data[i][j][k+1][0] = op(data[i][j][k][0], data[i+(1<<k)][j][k][0]);
            }
        }
        for(int l=0;l<_logm-1;l++){
            for(int i=0;i<_n;i++) for(int j=0;j+(1<<l)<_m;j++){
                data[i][j][0][l+1] = op(data[i][j][0][l], data[i][j+(1<<l)][0][l]);
            }
        }
        for(int k=0;k<_logn-1;k++) for(int l=0;l<_logm-1;l++){
            for(int i=0;i+(1<<k)<_n;i++) for(int j=0;j+(1<<l)<_m;j++){
                data[i][j][k+1][l+1] = op(data[i][j][k][l+1], data[i+(1<<k)][j][k][l+1]);
            }
        }
    }
    S prod(int p1, int p2, int q1, int q2){ //[(p1,p2), (q1,q2))
        unsigned int d1 = 32 - __builtin_clz((unsigned int)(q1 - p1)) - 1;
        unsigned int d2 = 32 - __builtin_clz((unsigned int)(q2 - p2)) - 1;
        assert(0<=p1 && p1<q1 && q1<=_n);
        assert(0<=p2 && p2<q2 && q2<=_m);
        return op(data[p1][p2][d1][d2], op(data[q1-(1<<d1)][p2][d1][d2], 
               op(data[p1][q2-(1<<d2)][d1][d2], data[q1-(1<<d1)][q2-(1<<d2)][d1][d2])));
    }
};
