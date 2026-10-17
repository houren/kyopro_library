struct relaxed_convolution{
    vector<mint> a, b, c;
    relaxed_convolution() : c(1){}
    int n = 0;
    void add_sub(int la, int ra, int lb, int rb){
        vector<mint> v = convolution(vector<mint>(begin(a)+la, begin(a)+ra), 
                                     vector<mint>(begin(b)+lb, begin(b)+rb));
        int m = la+lb;
        for(mint x:v) c[m++] += x;
    }
    void add(mint x, mint y){
        a.emplace_back(x);
        b.emplace_back(y);
        n++;
        int k = 0;
        for(int i=1;1;i<<=1){
            add_sub(k, k+i, n-i, n);
            if(k+i!=n) add_sub(n-i, n, k, k+i);
            else break;
            if(!(n&i)) break;
            k += i;
        }
        c.emplace_back(0);
        c.emplace_back(0);
    };
    mint get(int x){
        assert(x >= 0);
        if(x >= (int)c.size()) return 0;
        return c[x];
    }
};