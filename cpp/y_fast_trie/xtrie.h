#ifndef XFASTTRIE
#define XFASTTRIE

void printBits(size_t const size, void const * const ptr)
{
    unsigned char *b = (unsigned char*) ptr;
    unsigned char byte;
    int i, j;

    for (i=size-1;i>=0;i--)
    {
        for (j=7;j>=0;j--)
        {
            byte = (b[i] >> j) & 1;
            printf("%u", byte);
        }
    }
    puts("");
}

#include<unordered_map>
#include<vector>

template <class T, class A = std::allocator<T> >
class xfasttrie
{
private:
    typedef unsigned long long int ll;

    const ll inf = (ll)18446744073709551615;

    typedef A allocator_type;
    typedef typename A::value_type value_type;
    typedef typename A::reference reference;
    typedef typename A::const_reference const_reference;
    typedef typename A::difference_type difference_type;
    typedef typename A::size_type size_type;

    struct node
    {
        ll prev;
        ll next;
        T data;

        node() {prev = next = (ll)18446744073709551615;}

        node(ll p, ll s, T d)
        {
            prev = p;
            next = s;
            data = d;
        }
        };
    struct internal
    {
        unsigned char skip = 0;

        ll l, p;

        internal() {l = p = (ll)18446744073709551615;}
        };

    typedef std::unordered_map<ll, node> hashtable;

    typedef std::unordered_map<ll, internal> nodetable;

    ll max_m = 0;

    int h = 0;

    std::vector<nodetable> nodes;

    hashtable leafs;

    ll masks[65];

public:
    xfasttrie(ll m)
    {
        max_m = m;
        h = std::ceil(std::log2(max_m)) + 1;
        printf("H : %d\n", h);
        nodes.resize(h + 1);
        masks[0] = 1;
        for(int i = 1; i <= h; i++)
            masks[i] = masks[i - 1]<<1 | (ll)1;
        for(int i = 0; i <= h; i++)
            masks[i] = ~masks[i];
        insert(inf, T());
        printf("DOBRZE\n");
    }

    reference f(ll ind)
    {
        if(leafs.find(ind) == leafs.end())
            insert(ind, T());
        return leafs[ind].data;
    }

    ll find_ancestor(ll ind)
    {
        int l = 0;
        int p = h;
        while(p - l > 1)
        {
            int sr = (l + p) / 2;
            if(nodes[sr].find(ind & masks[sr]) == nodes[sr].end())
                l = sr;
            else
                p = sr;
        }
        if(nodes[l].find(ind & masks[l]) == nodes[l].end())
            l = p;
        if(nodes[l].find(ind & masks[l]) == nodes[l].end())
            return inf;
        ll mask = inf;
        if(l > 0)
            mask = masks[l - 1];
        if((ind & mask) != (ind & masks[l]))
            return nodes[l][ind & masks[l]].p;
        else
            return nodes[l][ind & masks[l]].l;
    }

    ll find_succesor(ll ind)
    {
        ll anc = find_ancestor(ind);
        //printf("ANC %llu\n", anc);
        if(anc <= ind && anc != inf)
            return leafs[anc].next;
        return anc;
    }

    ll find_predescesor(ll ind)
    {
        ll anc = find_ancestor(ind);
        if(anc >= ind && anc != inf)
            return leafs[anc].prev;
        return anc;
    }

    void insert(ll key, T data)
    {
        if(leafs.find(key) != leafs.end())
            return;
        ll pred = find_predescesor(key);
        ll succ = find_succesor(key);
        //printf("INS %llu %llu %llu\n", pred, succ, key);
        if(pred != inf)
            leafs[pred].next = key;
        if(succ != inf)
            leafs[succ].prev = key;
        leafs[key] = node(pred, succ, data);
        //printBits(sizeof(key), &key);
        for(int i = h; i >= 0; i--)
        {
            ll mask = inf;
            if(i > 0)
                mask = masks[i - 1];
            //printf("%llu ", (key & mask));
            if((key & mask) != (key & masks[i]))
            {
                nodes[i][key & masks[i]].p = key & mask;
                nodes[i][key & masks[i]].skip = nodes[i][key & masks[i]].skip & 1;
                 if(nodes[i][key & masks[i]].l == inf || ((nodes[i][key & masks[i]].skip & 1) != 0 && nodes[i][key & masks[i]].l > key))
                 {
                     nodes[i][key & masks[i]].skip = nodes[i][key & masks[i]].skip | 1;
                     nodes[i][key & masks[i]].l = key;
                 }
            }
            else
            {
                nodes[i][key & masks[i]].l = key & mask;
                nodes[i][key & masks[i]].skip = nodes[i][key & masks[i]].skip & 2;
                 if(nodes[i][key & masks[i]].p == inf || ((nodes[i][key & masks[i]].skip & 2) != 0 && nodes[i][key & masks[i]].p < key))
                 {
                     nodes[i][key & masks[i]].skip = nodes[i][key & masks[i]].skip | 2;
                     nodes[i][key & masks[i]].p = key;
                 }
            }
        }
    }

    ll erase(ll key)
    {
        if(leafs.find(key) == leafs.end())
            return inf;
        ll pred = leafs[key].prev;
        ll succ = leafs[key].next;
        if(pred != inf)
            leafs[pred].next = succ;
        if(succ != inf)
            leafs[succ].prev = pred;
        /*if(key == 119)
            printf("%d\n", leafs[key].data);*/
        leafs.erase(key);
        bool deleted = true;
        //printBits(sizeof(key), &key);
        for(int i = 0; i <= h; i++)
        {
            ll mask = inf;
            if(i > 0)
                mask = masks[i - 1];
            if((((nodes[i][key & masks[i]].skip & 2) != 0) &&
                (nodes[i][key & masks[i]].p == key)) || (deleted && ((key & mask) != (key & masks[i]))))
            {
                nodes[i][key & masks[i]].p = pred;
                nodes[i][key & masks[i]].skip = nodes[i][key & masks[i]].skip | 2;
            }
            if((((nodes[i][key & masks[i]].skip & 1) != 0) &&
                (nodes[i][key & masks[i]].l == key)) || (deleted && ((key & mask) == (key & masks[i]))))
            {
                nodes[i][key & masks[i]].l = succ;
                nodes[i][key & masks[i]].skip = nodes[i][key & masks[i]].skip | 1;
            }
            if(nodes[i][key & masks[i]].skip == 3)
            {
                nodes[i].erase(key & masks[i]);
                deleted = true;
            }
            else
                deleted = false;
        }
        return succ;
    }

    ~xfasttrie()
    {
        //printf("CLEANUP\n");
    }

    class iterator {
    public:
        struct bucket
        {
            ll first;
            T second;

            //bucket(const bucket& t) : first(t.first), second(t.second) {}

            bucket(ll& ind, T& d) : first(ind), second(d) {}
            };

    private:
        ll ind = 0;
        xfasttrie* tree = nullptr;

        bucket temp;

        public:
            typedef typename A::difference_type difference_type;
            typedef typename A::value_type value_type;
            typedef typename A::reference reference;
            typedef typename A::pointer pointer;
            typedef std::bidirectional_iterator_tag iterator_category;

            iterator() {}
            //iterator(const iterator& it) : temp(it.temp) {ind = it.ind; tree = it.tree;}
            iterator(ll x, xfasttrie* t) : temp(x, t->f(x)) {ind = x; tree = t;}
            //iterator(const iterator& it) {ind = it.ind;}
            ~iterator() {}

            iterator& operator=(const iterator& it){ind = it.ind; return *this;}
            bool operator==(const iterator& it) const{return ind == it.ind && tree == it.tree;}
            bool operator!=(const iterator& it) const{return ind != it.ind || tree != it.tree;}

            iterator& operator++(){ind = tree->find_succesor(ind); return *this;}
            iterator& operator--(){ind = tree->find_predescesor(ind); return *this;}
            iterator& operator++(int junk){iterator it(ind, tree); ind = tree->find_succesor(ind); return it;}
            iterator& operator--(int junk){iterator it(ind, tree); ind = tree->find_predescesor(ind); return it;}

            reference operator*()
            {
                return tree->find(ind);
            }

            bucket* operator->()
            {
                temp.first = ind;
                temp.second = tree->f(ind);
                return &temp;
            }
        };

        size_type size() const {return leafs.size() - 1;}

        bool empty() const {return leafs.size() <= 1;}

        iterator erase(iterator it)
        {
            return iterator(erase(it->first), this);
        }

        iterator lower_bound(ll ind)
        {
            if(leafs.find(ind) != leafs.end())
            {
                //printf("%llu\n\n", ind);
                return iterator(ind, this);
            }

            return iterator(find_succesor(ind), this);
        }

        iterator upper_bound(ll ind)
        {
            return iterator(find_succesor(ind), this);
        }

        iterator begin()
        {
            return lower_bound(0);
        }

        iterator end() {return iterator(inf, this);}

        iterator find(ll key) {if(leafs.find(key) == leafs.end()) return end(); return iterator(key, this);}

        reference operator[](ll ind)
        {
            return f(ind);
        }
    };

#endif // XFASTTRIE
