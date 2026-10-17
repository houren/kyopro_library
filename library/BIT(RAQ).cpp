template <typename T>
struct BIT{
    int n;
    vector<T> data[2];
    BIT(int _n=0) : n(_n){ init(); }
    void init(){
        rep(i,2) data[i].assign(n+1, 0);
    }
    T sum_sub(int p, int i){
        T res = 0;
        for(; i; i -= i&-i) res += data[p][i];
        return res;
    }
    T sum(int i){ return sum_sub(0,i) + sum_sub(1,i)*i; }
    void add_sub(int p, int i, T x){
        for(; i <= n; i += i&-i) data[p][i] += x;
    }
    //[l,r)
    void add(int l, int r, T x){
        add_sub(0, l, -x*(l-1));
        add_sub(0, r, x*(r-1));
        add_sub(1, l, x);
        add_sub(1, r, -x);
    }
};