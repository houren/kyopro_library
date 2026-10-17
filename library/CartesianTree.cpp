template <class S, bool (*comp)(S, S)>
struct CartesianTree{
    int n, root = -1;
    vector<int> l, r, par;
    CartesianTree(const vector<S>& a) : n(a.size()), l(n,-1), r(n,-1), par(n,-1){
        stack<int> sta;
        for(int i=0;i<n;++i){
            while(sta.size() && comp(a[i], a[sta.top()])){
                l[i] = sta.top();
                par[sta.top()] = l[i];
                sta.pop();
            }
            if(sta.size()){
                par[i] = sta.top();
                r[sta.top()] = i;
            }
            sta.emplace(i);
        }
        while(sta.size()>1) sta.pop();
        root = sta.top();
    }
};