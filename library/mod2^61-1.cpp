const unsigned long long MASK30 = (1UL << 30) - 1;
const unsigned long long MASK31 = (1UL << 31) - 1;
const unsigned long long MASK61 = (1UL << 61) - 1;
unsigned long long mul61(unsigned long long a, unsigned long long b){
    unsigned long long au = a >> 31, ad = a & MASK31, bu = b >> 31, bd = b & MASK31;
    unsigned long long mid = au * bu + au * bd, midu = mid >> 30, midd = mid & MASK30;
    return calc_mod_mask61(au * bu * 2 + midu + (midd << 31) + ad * bd);
}
unsigned long long calc_mod_mask61(unsigned long long x){
    unsigned long long res = (x >> 61) + (x & MASK61);
    if(res >= MASK61) res -= MASK61;
    return res;
}