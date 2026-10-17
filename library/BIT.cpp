template <typename T>
struct BIT{
    int n;
    vector<T> data;
    BIT(int n=0) : n(n), data(n+1){}
    T sum(int i){
        T res = 0;
        for(; i; i -= i&-i){
            res += data[i];
        }
        return res;
    }
    void add(int i, T x){
        for(; i <= n; i += i&-i){
            data[i] += x;
        }
    }
    int lower_bound(T w){
        int x = 0, r = 1;
        while(r<n) r <<= 1;
        for(int l=r;l>0;l>>=1){
            if(x+l<=n && data[x+l]<w){
                x += l;
                w -= data[x];
            }
        }
        return x+1;
    }
};