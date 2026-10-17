template<class T>
struct MongeMinimizer {
private:
    struct Edge {
        int from, to;
        T cap;
    };

    int n, K, S, G, V;
    
    vector<Edge> edges;
    T offset = 0, ans = 0, INF;
    bool solved = false;
    mf_graph<T> mf;

    int id(int i, int d) const {
        assert(0 <= i && i < n);
        assert(1 <= d && d < K);
        return i * (K - 1) + d - 1;
    }

    int new_node() {
        return V++;
    }

    void add_edge(int u, int v, T cap) {
        assert(cap >= 0);
        edges.emplace_back(u, v, cap);
    }

    void check_mutable() const {
        assert(!solved);
    }

    void add_bool_cost(int v, T c) {
        if (c >= 0) add_edge(v, G, c);
        else {
            offset += c;
            add_edge(S, v, -c);
        }
    }

public:
    MongeMinimizer(int n, int K, T INF = T(1 << 30)) : n(n), K(K), INF(INF) {
        assert(n >= 0);
        assert(K >= 1);
        assert(INF > 0);
        S = n * (K - 1);
        G = S + 1;
        V = G + 1;
        for (int i = 0; i < n; ++i) {
            for (int d = 1; d + 1 < K; ++d) {
                add_edge(id(i, d + 1), id(i, d), INF);
            }
        }
    }

    void add_unary(int i, const vector<T>& theta) {
        check_mutable();
        assert(0 <= i && i < n);
        assert((int)theta.size() == K);
        offset += theta[0];
        for (int d = 1; d < K; ++d) add_bool_cost(id(i, d), theta[d] - theta[d - 1]);
    }

    template<class F>
    void add_unary(int i, F theta) {
        vector<T> v(K);
        for (int x = 0; x < K; ++x) v[x] = theta(x);
        add_unary(i, v);
    }

    void add_pairwise(int i, int j, const vector<vector<T>>& phi) {
        check_mutable();
        assert(0 <= i && i < n);
        assert(0 <= j && j < n);
        assert(i != j);
        assert((int)phi.size() == K);
        for (const auto& row : phi) assert((int)row.size() == K);
        vector<T> ui(K), uj(K);
        for (int x = 0; x < K; ++x) ui[x] = phi[x][0];
        for (int y = 0; y < K; ++y) uj[y] = phi[0][y] - phi[0][0];
        add_unary(i, ui);
        add_unary(j, uj);
        for (int a = 1; a < K; ++a) {
            for (int b = 1; b < K; ++b) {
                T delta = phi[a][b] - phi[a - 1][b] - phi[a][b - 1] + phi[a - 1][b - 1];
                assert(delta <= 0);
                T c = -delta;
                if (c == 0) continue;
                add_bool_cost(id(i, a), -c);
                add_edge(id(i, a), id(j, b), c);
            }
        }
    }

    template<class F>
    void add_pairwise(int i, int j, F phi) {
        vector<vector<T>> v(K, vector<T>(K));
        for (int x = 0; x < K; ++x) {
            for (int y = 0; y < K; ++y) {
                v[x][y] = phi(x, y);
            }
        }
        add_pairwise(i, j, v);
    }

    void set(int i, int x) {
        check_mutable();
        assert(0 <= i && i < n);
        assert(0 <= x && x < K);
        for (int d = 1; d < K; ++d) {
            if (d <= x) add_edge(S, id(i, d), INF);
            else add_edge(id(i, d), G, INF);
        }
    }

    // 1つ以上がそれ未満ならばコストがかかる
    void add_cost_if_any_lt(const vector<pair<int, int>>& cond, T cost) {
        check_mutable();
        assert(cost >= 0);
        if (cost == 0) return;
        vector<int> vs;
        for (auto [i, d] : cond) {
            assert(0 <= i && i < n);
            if (d <= 0) continue;
            if (d >= K) {
                offset += cost;
                return;
            }
            vs.emplace_back(id(i, d));
        }
        if (vs.empty()) return;
        int z = new_node();
        add_edge(S, z, cost);
        for (int v : vs) add_edge(z, v, INF);
    }

    // 1つ以上がそれ以上ならばコストがかかる
    void add_cost_if_any_ge(const vector<pair<int, int>>& cond, T cost) {
        check_mutable();
        assert(cost >= 0);
        if (cost == 0) return;
        vector<int> vs;
        for (auto [i, d] : cond) {
            assert(0 <= i && i < n);
            if (d <= 0) {
                offset += cost;
                return;
            }
            if (d >= K) continue;
            vs.emplace_back(id(i, d));
        }
        if (vs.empty()) return;
        int z = new_node();
        for (int v : vs) add_edge(v, z, INF);
        add_edge(z, G, cost);
    }

    T solve() {
        if (solved) return ans;
        mf = mf_graph<T>(V);
        for (auto [u, v, cap] : edges) mf.add_edge(u, v, cap);
        ans = offset + mf.flow(S, G);
        solved = true;
        return ans;
    }

    vector<int> restore() {
        solve();
        auto cut = mf.min_cut(S);
        vector<int> x(n);
        for (int i = 0; i < n; ++i) {
            for (int d = 1; d < K; ++d) {
                if (cut[id(i, d)]) ++x[i];
            }
        }
        return x;
    }
};