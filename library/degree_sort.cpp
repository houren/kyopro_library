struct Point{
    ll x,y;
};
vector<int> degree_sort(vector<Point> v, ll x, ll y){
    vector<int> res;
    vector<vector<pair<Point,int>>> dec(4);
    int n = v.size();
    rep(i,n){
        v[i].x -= x, v[i].y -= y;
        if(!v[i].x && !v[i].y) continue;
        if(v[i].y>=0 && v[i].x>0) dec[0].push_back({v[i], i});
        else if(v[i].x<=0 && v[i].y>0) dec[1].push_back({{v[i].y, -v[i].x}, i});
        else if(v[i].y<=0 && v[i].x<0) dec[2].push_back({{-v[i].x, -v[i].y}, i});
        else dec[3].push_back({{-v[i].y, v[i].x}, i});
    }
    rep(i,4){
        sort(all(dec[i]), [](pair<Point,int> a, pair<Point,int> b){
            return a.first.y*b.first.x < b.first.y*a.first.x;
        });
        for(auto &p:dec[i]) res.emplace_back(p.second);
    }
    return res;
}

ld degree(Point p, Point q, ll x, ll y){
    p.x -= x, q.x -= x, p.y -= y, q.y -= y;
    ld res = (atan2((ld)(q.y), (ld)(q.x)) - atan2((ld)(p.y), (ld)(p.x))) * 180 / PI;
    if(res<0) res += 360;
    return res;
}
