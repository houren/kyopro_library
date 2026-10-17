// 2n:right move, 2n+1:left move
vector<long long> sbt_encode_path(long long a, long long b){
    vector<long long> res;
    while(b){
        res.emplace_back(a/b);
        a %= b;
        swap(a, b);
    }
    return res;
}
pair<long long, long long> sbt_decode_path(vector<long long> v){
    long long p = 0, q = 1, r = 1, s = 0;
    if(v.size()&1) v.emplace_back(0);
    for(int i=0;i<(int)v.size();i+=2){
        p += v[i]*r;
        q += v[i]*s;
        r += v[i+1]*p;
        s += v[i+1]*q;
    }
    return make_pair(p+r, q+s);
}
// a/b, c/d
pair<long long, long long> sbt_lca(long long a, long long b, long long c, long long d){
    auto p = sbt_encode_path(a, b);
    auto q = sbt_encode_path(c, d);
    vector<long long> path;
    int n = min(p.size(), q.size());
    for(int i=0;i<n;++i){
        if(p[i]!=q[i]){
            path.emplace_back(min(p[i], q[i]));
            break;
        }
        path.emplace_back(p[i]);
    }
    return sbt_decode_path(path);
}
//depth:k
pair<long long, long long> sbt_ancestor(long long k, long long a, long long b){
    auto path = sbt_encode_path(a,b);
    int n = path.size();
    for(int i=0;i<n&&k>0;++i){
        if(path[i]>=k){
            path[i] = k;
            while(i+1<(int)path.size()) path.pop_back();
        }
        k -= path[i];
    }
    if(k>0) return make_pair(-1, 0);
    return sbt_decode_path(path);
}