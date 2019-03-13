/*
        Author: Jan Jurkowski
        Under GPLv3
        Example of implementation of Y-Fast Tries,
        which allows to get successor and predecessor
        of any element in log log M where M is the maximum
        range of the keys. Every operation of this Trie is
        done in log log M or constant time, and memory is linear
                    */

#include<cstdio>
#include<cmath>
#include "xtrie.h"
#include<cstdlib>
#include<ctime>
#include<map>

template <class T, class A = std::allocator<T> >
class yfasttrie
{
    typedef unsigned long long int ll;

    const ll inf = (ll)18446744073709551615;

    xfasttrie<std::map<ll, T> > structure;

    int h = 0;

    typedef A allocator_type;
    typedef typename A::value_type value_type;
    typedef typename A::reference reference;
    typedef typename A::const_reference const_reference;
    typedef typename A::difference_type difference_type;
    typedef typename A::size_type size_type;
public:
    yfasttrie(ll m) : structure(m) {h = std::ceil(std::log2(m)) + 1;}

    yfasttrie() {}

    void split(ll rep)
    {
        //printf("SPLIT %d\n", (int)structure[rep].size());
        if(structure[rep].size() > 2 * h)
        {
            std::map<ll, T>& get = structure[rep];
            printf("SPLIT2 %d\n", get.size());
            std::map<ll, T> temp;
            std::map<ll, T> temp2;
            int length = 0;
            for(auto it = get.begin(); it != get.end(); ++it)
            {
                if(length > h)
                    temp2[it->first] = it->second;
                else
                    temp[it->first] = it->second;
                length++;
            }
            structure.erase(rep);
            structure[temp.rbegin()->first] = temp;
            structure[temp2.rbegin()->first] = temp2;
        }
    }

    void insert(ll key)
    {
        auto it = structure.lower_bound(key);
        if(it == structure.end())
        {
            structure.insert(key, std::map<ll, T>());
            //printf("IND %llu %llu\n", key, structure.lower_bound(key)->first);
            it = structure.find(key);
        }
        //it->second[key] = T();
        //if(!it->second.empty())
        split(it->first);
    }

    ll merge(ll a, ll b)
    {
        std::map<ll, T>& temp1 = structure[a];
        std::map<ll, T>& temp2 = structure[b];
        temp2.insert(temp1.begin(), temp1.end());
        structure.erase(a);
        return b;
    }

    ll find_succesor(ll key)
    {
        auto it = structure.upper_bound(key);
        auto it2 = it->second.upper_bound(key);
        if(it2 != it->second.end())
            return it2->first;
        it++;
        if(it == structure.end())
            return inf;
        it2 = it->second.upper_bound(key);
        if(it2 == it->second.end())
            return inf;
        return it2->first;
    }

    ll erase(ll key)
    {
        auto it = structure.lower_bound(key);
        it->second.erase(key);
        if(it->second.size() * 2 < h)
        {
            auto it2 = it;
            ++it2;
            if(it2 != structure.end())
            {
                split(merge(it->first, it2->first));
            }
            else
            {
                it2 = it;
                it2--;
                if(it2 != structure.end())
                {
                    split(merge(it->first, it2->first));
                }
            }
        }
        return find_succesor(key);
    }

    reference find(ll key)
    {
        auto it = structure.lower_bound(key);
        printf("FIND %llu %llu\n", key, it->first);
        if(it == structure.end() || it->second.find(key) == it->second.end())
        {
            insert(key);
            it = structure.lower_bound(key);
        }
        //printf(" %llu\n", it->first);
        return it->second[key];
    }

    reference operator[](ll key)
    {
        return find(key);
    }

    ~yfasttrie() {}
    };

unsigned long long int lrand()
{
    return ((unsigned long long int)rand()) * ((unsigned long long int)RAND_MAX) * ((unsigned long long int)RAND_MAX) + ((unsigned long long int)rand()) * ((unsigned long long int)RAND_MAX) + ((unsigned long long int)rand());
}

int main()
{
    yfasttrie<int> test((unsigned long long int)1000000000000000000);
    //std::map<unsigned long long int, int> test;
    srand(7432974);
    //printf("%d\n", RAND_MAX);
    /*test.insert(0, 0);
    test.insert(2, 2);
    printf("WYNIK %llu %llu\n", test.find_predescesor(1), test.find_succesor(1));*/
    std::map<unsigned long long int, int> kol;
    for(int i = 20000; i >= 0; i--)
    {
        unsigned long long int ran = i;//lrand();
        int ran2 = rand();
        //printf("%llu ", ran);
        //if(kol.find(ran) == kol.end())
        {
            kol[ran] = ran2;
            test[ran] = ran2;
        }
    }
    for(auto it = kol.begin(); it != kol.end(); ++it)
    {
        printf("%llu %d %d\n", it->first, it->second, test[it->first]);
        if(it->second != test[it->first])
        {
            printf("DUPA %llu %d %d\n", it->first, it->second, test[it->first]);
            return 0;
        }
    }
    printf("WYNIK\n");
    /*auto it = test.begin();
    while(it != test.end())
    {
        //printf("%llu ", it->first);
        if(kol.find(it->first) == kol.end())
        {
            printf("%llu ", it->first);
            printf("DUPA\n");
            return 0;
        }
        kol.erase(it->first);
        it = test.erase(it);
        //++it;
    }
    printf("\n\n");
    if(!test.empty())
        printf("DUPA1\n");
    if(!kol.empty())
        printf("DUPA2\n");*/
    /*for(auto it2 = test.begin(); it2 != test.end(); ++it2)
        printf("%llu ", it2->first);
    for(auto it2 = kol.begin(); it2 != kol.end(); it2++)
        printf("%llu ", it2->first);*/
    return 0;
}
