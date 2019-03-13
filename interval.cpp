/*
        Author: Jan Jurkowski
        Under GPLv3
        Example of an interval tree, implemented in a functional style
        and using pointers, can be easily modified to work with clear C
        Allows you to add and make queries on an interval, uses only
        n * log m memory, where n is number of insertions and m is the
        maximum range of queries
                    */

#include<cstdio>
#include<algorithm>

struct node
{
    node* l = nullptr;
    node* r = nullptr;
    int p, k;
    int v = 0;
    int sum = 0;

    node(int p1, int k1)
    {
        p = p1;
        k = k1;
    }
    };

node* root;

void insert_v(int x, node* wierz)
{
    if(wierz->k - wierz->p > 0)
    {
        int sr = (wierz->p + wierz->k) / 2;
        if(wierz->l == nullptr)
        {
            wierz->l = new node(wierz->p, sr);
            wierz->r = new node(sr + 1, wierz->k);
        }
        if(x <= sr)
        {
            insert_v(x, wierz->l);
            //wierz->r->v += v;
        }
        else
        {
            insert_v(x, wierz->r);
            //wierz->l->v += v;
        }
        //wierz->sum = wierz->l->sum + wierz->r->sum + (wierz->k - wierz->p + 1) * wierz->v;
    }
    /*else
    {
        wierz->v += v;
        wierz->sum += v;
    }*/
}

void rep(int x, node* wierz)
{
    if(wierz->k - wierz->p > 0)
    {
        if((wierz->p + wierz->k) / 2 >= x)
        {
            rep(x, wierz->l);
            //wierz->r->v += v;
        }
        else
        {
            rep(x, wierz->r);
            //wierz->l->v += v;
        }
        wierz->sum = std::max(wierz->r->sum, wierz->l->sum) + wierz->v;
    }
}

void insert(int l, int p, int v)
{
    insert_v(l, root);
    insert_v(p, root);
    node* a = root;
    node* b = root;
    while(a->k - a->p > 0)
    {
        printf("%d %d %d %d\n", a->p, a->k, b->p, b->k);
        if(a != b)
        {
            if((a->k + a->p) / 2 >= l)
                a->r->v += v;
            if((b->k + b->p) / 2 < p)
                b->l->v += v;
        }
        if((a->k + a->p) / 2 >= l)
        {
            a = a->l;
        }
        else
        {
            a = a->r;
        }
        if((b->k + b->p) / 2 < p)
        {
            b = b->r;
        }
        else
        {
            b = b->l;
        }
    }
    a->v += v;
    a->sum += v;
    if(a != b)
    {
        b->v += v;
        b->sum += v;
    }
    rep(l, root);
    rep(p, root);
}

int querry(int a, int b)
{
    int wynik = 0;

}

void dfs_print(node* wierz)
{
    if(wierz->l != nullptr)
    {
        dfs_print(wierz->l);
        dfs_print(wierz->r);
    }
    printf("%d %d %d| ", wierz->p, wierz->k, wierz->v);
}

int main()
{
    root = new node(0, (1<<4) - 1);
    insert(0, 10, 5);
    insert(6, 12, -3);
    dfs_print(root);
    return 0;
}
