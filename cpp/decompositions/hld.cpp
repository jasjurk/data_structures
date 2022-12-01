/*
        Author: Jan Jurkowski
        Under GPLv3
        Example of heavy-light decomposition
        of a tree, which allows you to decompose
        a path of a tree into a logarithmically
        many contiguous linear paths, which is why
        you can use RMQ algorithms to operate on a tree
                    */

#include<cstdio>
#include<vector>
#include<queue>
#include<algorithm>
#include<cstring>

const int max_n = 1<<20;

int pocz_kubka[max_n];

int index[max_n];

std::vector<int> kraw[max_n];

bool odw[max_n];

std::queue<int> kolej;

int parent[max_n];

int dzieci[max_n];

int tree[2 * max_n];

void insert(int x, int v)
{
    x += max_n;
    tree[x] = v;
    x /= 2;
    while(x > 0)
    {
        tree[x] = std::__gcd(tree[x * 2], tree[2 * x + 1]);
        x /= 2;
    }
}

int querry(int a, int b)
{
    a += max_n;
    b += max_n;
    int wynik = std::__gcd(tree[a], tree[b]);
    while(b - a > 1)
    {
        if(a % 2 == 0)
            wynik = std::__gcd(wynik, tree[a + 1]);
        if(b % 2 == 1)
            wynik = std::__gcd(wynik, tree[b - 1]);
        b /= 2;
        a /= 2;
    }
    return wynik;
}

int pre_dfs(int x)
{
    odw[x] = true;
    for(unsigned i = 0; i < kraw[x].size(); i++)
    {
        if(!odw[kraw[x][i]])
        {
            dzieci[x] += pre_dfs(kraw[x][i]);
        }
    }
    return dzieci[x] + 1;
}

void dfs(int x, int &czas, int pocz)
{
    odw[x] = true;
    index[x] = czas;
    czas++;
    pocz_kubka[x] = pocz;
    int maks = 0;
    for(unsigned i = 0; i < kraw[x].size(); i++)
    {
        if(!odw[kraw[x][i]] && maks < dzieci[kraw[x][i]])
        {
            maks = dzieci[kraw[x][i]];
        }
    }
    for(unsigned i = 0; i < kraw[x].size(); i++)
    {
        if(!odw[kraw[x][i]])
            parent[kraw[x][i]] = x;
        if(!odw[kraw[x][i]] && maks > dzieci[kraw[x][i]])
        {
            kolej.push(kraw[x][i]);
        }
        else if(!odw[kraw[x][i]] && maks == dzieci[kraw[x][i]])
        {
            dfs(kraw[x][i], czas, pocz);
            maks = 1<<30;
        }
    }
}

void hld(int n, int x)
{
    parent[x] = -1;
    pre_dfs(x);
    for(int i = 0; i < n; i++)
        odw[i] = false;
    int czas = 0;
    kolej.push(x);
    while(!kolej.empty())
    {
        printf("KOLEJ %d\n", kolej.front());
        dfs(kolej.front(), czas, czas);
        kolej.pop();
    }
}

int przejdz(int x, int cel)
{
    cel = index[cel];
    int l = pocz_kubka[x];
    int p = index[x];
    int wynik = 0;
    while(cel > p || cel < l)
    {
        printf("%d %d\n", l, p);
        wynik = std::__gcd(wynik, querry(l, p));
        x = parent[x];
        l = pocz_kubka[x];
        p = index[x];
    }
    if(cel <= p && cel >= l)
        l = cel;
    wynik = std::__gcd(wynik, querry(l, p));
    printf("%d %d\n", l, p);
    return wynik;
}

int main()
{
    int n;
    scanf("%d", &n);
    for(int i = 0; i < n - 1; i++)
    {
        int a, b;
        scanf("%d %d", &a, &b);
        kraw[a].push_back(b);
        kraw[b].push_back(a);
    }
    hld(n, 0);
    for(int i = 0; i < n; i++)
    {
        printf("%d %d %d\n", pocz_kubka[i], index[i], dzieci[i]);
    }
    while(true)
    {
        char slowo[256];
        scanf("%s", slowo);
        if(strcmp(slowo, "update") == 0)
        {
            int a, v;
            scanf("%d %d", &a, &v);
            insert(index[a], v);
        }
        if(strcmp(slowo, "querry") == 0)
        {
            int a, b;
            scanf("%d %d", &a, &b);
            printf("%d\n", przejdz(a, b));
        }
        if(strcmp(slowo, "exit") == 0)
            return 0;
    }
    return 0;
}
