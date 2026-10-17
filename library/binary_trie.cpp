struct BinaryTrie{
    vector<int> v;
    vector<vector<int>> trie;
    vector<long long> cnt;
    int d;
    BinaryTrie(int d_) : d(d_){
        v = vector<int>(2, -1);
        trie = vector<vector<int>>(1, v);
        cnt = vector<long long>(1, 0);
    }
    int count(long long x){
        int now = 0, k;
        for(int i=d-1;i>=0;i--){
            k = (x>>i)&1;
            if(trie[now][k]<0) return 0;
            now = trie[now][k];
        }
        return cnt[now];
    }
    // xをy個追加・削除
    void add(long long x, long long y){
        int now = 0, k;
        cnt[0] += y;
        for(int i=d-1;i>=0;i--){
            k = (x>>i)&1;
            if(trie[now][k]<0){
                trie[now][k] = trie.size();
                trie.push_back(v);
                cnt.push_back(0);
            }
            now = trie[now][k];
            cnt[now] += y;
        }
    }
    // 全て削除できない場合は可能な分だけ削除
    long long erase_sub(long long x, long long y, int i, int now){
        if(i<0){
            long long res = min(cnt[now], y);
            cnt[now] -= res;
            return res;
        }
        int k = (x>>i)&1;
        if(trie[now][k]<0) return 0;
        long long res = erase_sub(x, y, i-1, trie[now][k]);
        cnt[now] -= res;
        return res;
    }
    long long erase(long long x, long long y){
        return erase_sub(x, y, d-1, 0);
    }
    long long xor_min(long long x){
        ll res = 0;
        int now = 0, k;
        for(int i=d-1;i>=0;i--){
            k = (x>>i)&1;
            if(trie[now][k]<0 || !cnt[trie[now][k]]){
                k ^= 1;
                res += 1LL << i;
            }
            now = trie[now][k];
            assert(now>=0 || cnt[now]>0);
        }
        return res;
    }
};
