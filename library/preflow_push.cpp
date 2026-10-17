template <typename T>
struct Preflow_Push{
    int n, m = 0;
    struct Edge{
        int to;
        T cap, flow;
        int re;
        Edge(int _to, T _cap, T _flow, int _re) : to(_to), cap(_cap), flow(_flow), re(_re){}
    };
    struct EdgeList{
        int u, v, u_edge, v_edge;
        EdgeList(int _u, int _v, int ue, int ve) : u(_u), v(_v), u_edge(ue), v_edge(ve){}
    };

    vector<vector<Edge>> g;
    vector<EdgeList> edge;
    vector<T> f;
    Preflow_Push(int _n) : n(_n), g(_n){}

    int add_edge(int u, int v, T cap){
        assert(0 <= u && u < n && 0 <= v && v < n);
        assert(u != v);
        assert(cap >= 0);
        edge.emplace_back(EdgeList(u, v, g[u].size(), g[v].size()));
        g[u].emplace_back(Edge(v, cap, 0,   g[v].size()));
        g[v].emplace_back(Edge(u, cap, cap, g[u].size()-1));
        return m++;
    }

    bool Push(int u, Edge& e, T x){
        if(!x) return false;
        e.flow += x;
        f[e.to] += x;
        Edge& re = g[e.to][e.re];
        re.flow -= x;
        f[u] -= x;
        return (f[e.to] == x);
    }
    
    T flow(int s, int t){
        assert(0 <= s && s < n && 0 <= t && t < n);
        vector<int> label(n, 0);
        vector<stack<int>> label_list(2*n);
        vector<stack<Edge*>> task(n);
        f = vector<T>(n, 0);
        label[s] = n;
        for(Edge &e : g[s]){
            if(Push(s, e, e.cap - e.flow) && e.to != t) label_list[0].push(e.to);
        }
        int now = 0, u, k, mi;
        Edge* v;
        while(true){
            while(now >= 0 && !label_list[now].size()) now--;
            if(now < 0) break;
            u = label_list[now].top();
            label_list[now].pop();
            while(f[u]){
                if(!task[u].size()){
                    mi = 2 * n;
                    for(Edge& e : g[u]){
                        if(e.cap > e.flow) mi = min(mi, label[e.to]);
                    }
                    for(Edge& e : g[u]){
                        if(e.cap > e.flow && mi == label[e.to]) task[u].push(&e);
                    }
                    label[u] = mi + 1;
                }
                while(task[u].size() && f[u]){
                    v = task[u].top(), k = min(f[u], v->cap - v->flow);
                    if(label[v->to]+1 != label[u]){
                        task[u].pop();
                        continue;
                    }
                    if(Push(u, *v, k)){
                        if(v->to != s && v->to != t) label_list[label[v->to]].push(v->to);
                    }
                    if(v->cap == v->flow) task[u].pop();
                }
            }
            now = label[u];
        }
        return f[t];
    }

    T get_edge(int i){ return g[edge[i].u][edge[i].u_edge].flow; }
    vector<T> edges(){
        vector<T> res(m);
        for(int i = 0; i < m; i++) res[i] = get_edge(i);
        return res;
    }
};