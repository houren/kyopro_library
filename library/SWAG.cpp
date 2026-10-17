template <class S, S (*op)(S, S), S (*e)()>
struct SWAG{
    vector<S> v;
    deque<int> d;
    int l = 0;
    void clear(){
        l = 0;
        v.clear();
        d.clear();
    }
    void push(S a){
        while(d.size() && op(v[d.back()], a)==a) d.pop_back();
        d.emplace_back(v.size());
        v.emplace_back(a);
    }
    S prod(int l_){
        assert(l<=l_);
        l = l_;
        while(d.size() && d.front()<l) d.pop_front();
        return d.size() ? v[d.front()] : e();
    }
};