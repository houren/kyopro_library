template <class S, S (*op)(S, S), S (*e)()>
struct UndoableUnionFind{
    int num;
    vector<int> data;
    vector<S> val;
    stack<tuple<int,int,S>> hist;
    UndoableUnionFind() = default;
    explicit UndoableUnionFind(int N) : num(N), data(N, -1), val(N, e()){}
    explicit UndoableUnionFind(const vector<S>& v) : num(v.size()), data(num, -1), val(v){}
    int root(int x){ return data[x] < 0 ? x : root(data[x]); }
    bool unite(int x,int y){
        x = root(x); y = root(y);
        hist.emplace(-1, num, e());
        hist.emplace(x, data[x], val[x]);
        hist.emplace(y, data[y], val[y]);
        if(x==y) return false;
        if(data[x] > data[y]) swap(x, y);
        data[x] += data[y];
        data[y] = x;
        val[x] = op(val[x], val[y]);
        return true;
    }
    int size(int x){
        return -data[root(x)];
    }
    bool same(int x,int y){
        return root(x) == root(y);
    }
    bool undo(){
        if(!hist.size()) return false;
        while(1){
            auto [x, y, z] = hist.top();
            hist.pop();
            if(x < 0){
                num = y;
                break;
            }
            data[x] = y, val[x] = z;
        }
        return true;
    }
    S prod(int x){ return val[root(x)]; }
    void update(int x, const S& v){
        x = root(x);
        hist.emplace(-1, num, e());
        hist.emplace(x, data[x], val[x]);
        val[x] = op(val[x], v);
    }
};