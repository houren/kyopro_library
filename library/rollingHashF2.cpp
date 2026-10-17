using u128 = unsigned __int128;
const int MAX_N = 5<<17;
uint64_t pow_tbl[MAX_N], inv_tbl[MAX_N];
constexpr uint64_t poly_f = 0x0000000247F43CB7ULL; // f(2,64)-x^64
const int D = 64;
constexpr uint64_t g = 123456789;
int deg(u128 x){
    if (x == 0) return -1;
    uint64_t hi = (uint64_t)(x >> 64);
    return (hi ? 127 - __builtin_clzll(hi) : 63 - __builtin_clzll((uint64_t)x));
}
uint64_t poly_inverse(uint64_t val){
    if(val==0) return 0;
    u128 m = ((u128)1 << D) | (u128)poly_f;
    u128 a = m, u = 0, b = val, v = 1;
    while(b){
        int da = deg(a), db = deg(b);
        if(da<db) swap(a,b), swap(u,v), swap(da,db);
        a ^= b << (da - db);
        u ^= v << (da - db);
    }
    for(int i=127;i>=D;--i){
        if(u>>i) u ^= m << (i-D);
    }
    return u;
}
uint64_t polymul(uint64_t a, uint64_t b){
    uint64_t res = 0;
    while(b){
        if(b&1) res ^= a;
        b >>= 1;
        a = (a << 1) ^ (poly_f * ((a >> (D - 1)) & 1));
    }
    return res;
}
void init(){
    pow_tbl[0] = inv_tbl[0] = 1;
    uint64_t ginv = poly_inverse(g);
    for(int i=1;i<MAX_N;++i) pow_tbl[i] = polymul(pow_tbl[i-1], g);
    for(int i=1;i<MAX_N;++i) inv_tbl[i] = polymul(inv_tbl[i-1], ginv);
}
uint64_t rollingHash(int l, int r, uint64_t* tbl){
    return polymul(inv_tbl[l], tbl[l] ^ tbl[r]);
}

uint64_t hash_tbl[MAX_N];