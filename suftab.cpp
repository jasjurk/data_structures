/*
        Author: Jan Jurkowski
        Under GPLv3
        Example of implementation of suffix array,
        structure which allows you to quickly find how
        many suffixes contain a pattern, this is done just
        for educational purposes, it is done in purely
        procedural style and is simple to understand,
        however slow, because it is done in O(n * log(n)^2)
        and there is also somewhat simple algorithm to
        compute this structure in O(n)
                    */

#include<cstdio>
#include<algorithm>
#include<cstring>

typedef long long ll;

const int max_n = 1000000;

const ll pod = 997;
const ll mod = 1000000009;

int n;

ll hash[max_n];
ll pods[max_n];
int suftab[max_n];
char slowo[2 * max_n] = {0};

bool cmphash(int i, int i2, int l)
{
    if(i > i2)
        std::swap(i, i2);
    if(l > n - i + 1 || l > n - i2 + 1)
        return false;
    ll hash1 = hash[i2 + l];
    if(i2 > 0)
        hash1 = (hash1 - ((hash[i2 - 1] * pods[l + 1]) % mod)) % mod;
    if(hash1 < 0)
        hash1 += mod;
    ll hash2 = hash[i + l];
    if(i > 0)
        hash2 = (hash2 - ((hash[i - 1] * pods[l + 1]) % mod)) % mod;
    if(hash2 < 0)
        hash2 += mod;
    return hash1 == hash2;
}

bool sufcmp(int i, int i2)
{
    int l = 0, p = n;
    while(p - l > 1)
    {
        int sr = (l + p) / 2;
        if(cmphash(i, i2, sr))
            l = sr;
        else
            p = sr;
    }
    if(!cmphash(i, i2, l))
        p = l;
    /*if(i + p >= n || i2 + p >= n)
        return i < i2;*/
    return slowo[i + p] < slowo[i2 + p];
}

void pre_compute()
{
    hash[0] = slowo[0];
    pods[0] = 1;
    for(int i = 1; i < n; i++)
    {
        pods[i] = (pods[i - 1] * pod) % mod;
        hash[i] = (hash[i - 1] * pod + (ll)slowo[i]) % mod;
    }
}

int main()
{
    scanf("%s", slowo);
    n = strlen(slowo);
    pre_compute();
    /*for(int i = 0; i < n; i++)
        printf("%lld %lld\n", hash[i], pods[i]);*/
    for(int i = 0; i < n; i++)
        suftab[i] = i;
    std::sort(suftab, suftab + n, sufcmp);
    for(int i = 0; i < n; i++)
        printf("%d ", suftab[i]);
    while(true)
    {
        int pocz, kon;
        char wzor[max_n] = {0};
        scanf("%s", wzor);
        int m = strlen(wzor);
        int l = 0, p = n - 1;
        while(p - l > 1)
        {
            printf("BINSEARCH %d %d\n", l, p);
            int sr = (l + p) / 2;
            if(strcmp(slowo + suftab[sr], wzor) == 1)
                l = sr;
            else
                p = sr;
        }
        if(strcmp(slowo + suftab[p], wzor) == 1)
            pocz = l;
        else
            pocz = p;
        l = 0;
        p = n - 1;
        while(p - l > 1)
        {
            int sr = (l + p) / 2;
            if(strcmp(slowo + suftab[sr], wzor) == -1)
                p = sr;
            else
                l = sr;
        }
        if(strcmp(slowo + suftab[l], wzor) == -1)
            kon = p;
        else
            kon = l;
        printf("%d %d\n", suftab[pocz], suftab[kon]);
    }
    return 0;
}
