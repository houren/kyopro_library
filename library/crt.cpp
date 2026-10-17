pair<long long, long long> inv_gcd(long long a, long long b) {
    a = (a + b) % b;
    if (a == 0) return {b, 0};
    long long s = b, t = a;
    long long m0 = 0, m1 = 1;
    while (t) {
        long long u = s / t;
        s -= t * u;
        m0 -= m1 * u;
        auto tmp = s;
        s = t;
        t = tmp;
        tmp = m0;
        m0 = m1;
        m1 = tmp;
    }
    if (m0 < 0) m0 += b / s;
    return {s, m0};
}

// (rem, mod)
pair<long long, long long> crt(vector<long long>& r, vector<long long>& m) {
    assert(r.size() == m.size());
    int n = int(r.size());
    long long r0 = 0, m0 = 1;
    for (int i = 0; i < n; i++) {
        assert(1 <= m[i]);
        long long r1 = (r[i] + m[i])%m[i], m1 = m[i];
        if (m0 < m1) {
            swap(r0, r1);
            swap(m0, m1);
        }
        if (m0 % m1 == 0) {
            if (r0 % m1 != r1) return {0, 0};
            continue;
        }
        long long g, im;
        tie(g, im) = inv_gcd(m0, m1);
        long long u1 = (m1 / g);
        if ((r1 - r0) % g) return {0, 0};
        long long x = (r1 - r0) / g % u1 * im % u1;
        r0 += x * m0;
        m0 *= u1;
        if (r0 < 0) r0 += m0;
    }
    return {r0, m0};
}

//xy=1 mod m
long long inv_mod(long long x, long long m) {
    assert(1 <= m);
    auto z = inv_gcd(x, m);
    assert(z.first == 1);
    return z.second;
}

// \sum_{i=0}^{n-1} {(ai+b)/m}
long long floor_sum(long long n, long long m, long long a, long long b) {
    long long ans = 0;
    if (a >= m) {
        ans += (n - 1) * n * (a / m) / 2;
        a %= m;
    }
    if (b >= m) {
        ans += n * (b / m);
        b %= m;
    }
    long long y_max = (a * n + b) / m, x_max = (y_max * m - b);
    if (y_max == 0) return ans;
    ans += (n - (x_max + a - 1) / a) * y_max;
    ans += floor_sum(y_max, a, m, (a - x_max % a) % a);
    return ans;
}