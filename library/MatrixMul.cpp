template <typename T>
vector<vector<T>> MatrixMul(vector<vector<T>> V1, vector<vector<T>> V2, T e){
    assert(V1[0].size()==V2.size());
    int N = V1.size(), M = V2[0].size(), L = V2.size();
    vector<vector<T>> res(N,vector<T>(M,e));
    rep(i,N) rep(j,M) rep(k,L) res[i][j] += V1[i][k] * V2[k][j];
    return res;
}
