/*
        Author: Jan Jurkowski
        Under GPLv3
        Example of treap implementation in purely procedural style
        using rotations. Split, find, insert is done in O(log n)
        Memory complexity is O(n), however this is an expected time,
        because it uses random to achieve this time.
                    */

#include<cstdio>
#include<cstdlib>
#include<cstring>

struct node
{
    node* l = nullptr;
    node* r = nullptr;
    int p = 0;
    int v = 0;
    int wielkosc = 1;

    node() {p = rand();}

    node(int tv) {p = rand(); v = tv;}
    };

node* rotl(node* parent, node* son)
{
    node* temp = son->r;
    son->r = parent;
    parent->l = temp;
    return son;
}

node* rotr(node* parent, node* son)
{
    node* temp = son->l;
    son->l = parent;
    parent->r = temp;
    return son;
}

node* insert(node* wierz, int v)
{
    if(wierz == nullptr)
        return new node(v);
    if(v > wierz->v)
        wierz->r = insert(wierz->r, v);
    else
        wierz->l = insert(wierz->l, v);
    if(wierz->l != nullptr && wierz->p > wierz->l->p)
        return rotl(wierz, wierz->l);
    if(wierz->r != nullptr && wierz->p > wierz->r->p)
        return rotr(wierz, wierz->r);
    return wierz;
}

node* remove(node* wierz, int v)
{
    if(wierz == nullptr)
        return nullptr;
    if(wierz->v == v)
    {
        if(wierz->l == nullptr)
        {
            node* temp = wierz->r;
            delete wierz;
            return temp;
        }
        if(wierz->r == nullptr)
        {
            node* temp = wierz->l;
            delete wierz;
            return temp;
        }
        if(wierz->r->p > wierz->l->p)
        {
            wierz = rotl(wierz, wierz->l);
            wierz->l = remove(wierz->l, v);
            return wierz;
        }
        else
        {
            wierz = rotl(wierz, wierz->r);
            wierz->r = remove(wierz->r, v);
            return wierz;
        }
    }
    else
    {
        if(wierz->v > v)
            wierz->l = remove(wierz->l, v);
        else
            wierz->r = remove(wierz->r, v);
        return wierz;
    }
}

/*node* split(node* wierz, int v)
{
    if(wierz == nullptr)
        return nullptr;
    if(wierz->v == v)
    {
        wierz->p = 0;
        return wierz;
    }
    if(v > wierz->v)
        wierz->r = remove(wierz->r, v);
    else
        wierz->l = remove(wierz->l, v);
    if(wierz->l != nullptr && wierz->p > wierz->l->p)
        return rotl(wierz, wierz->l);
    if(wierz->r != nullptr && wierz->p > wierz->r->p)
        return rotr(wierz, wierz->r);
    return wierz;
}*/

/*node* rem(node* wierz)
{
    if(wierz->l != nullptr && wierz->r != nullptr)
    {
        if(wierz->l->p <= wierz->r->p)
    }

}*/

node* root = nullptr;

void del(node* wierz)
{
    if(wierz == nullptr)
        return;
    printf("%d ", wierz->v);
    del(wierz->l);
    del(wierz->r);
    delete wierz;
}

void print(node* wierz)
{
    if(wierz == nullptr)
    {
        printf("|\n");
        return;
    }
    printf("%d %d\n", wierz->p, wierz->v);
    print(wierz->l);
    print(wierz->r);
}

bool find(node* wierz, int v)
{
    if(wierz == nullptr)
        return false;
    if(wierz->v == v)
        return true;
    if(wierz->v < v)
        return find(wierz->r, v);
    else
        return find(wierz->l, v);
}

int main()
{
    while(true)
    {
        char s[256];
        scanf("%s", s);
        if(strcmp(s, "insert") == 0)
        {
            int v;
            scanf("%d", &v);
            root = insert(root, v);
        }
        if(strcmp(s, "print") == 0)
        {
            /*int v;
            scanf("%d", &v);
            printf("%d\n", find(root, v));*/
            print(root);
        }
        if(strcmp(s, "find") == 0)
        {
            int v;
            scanf("%d", &v);
            printf("%d\n", find(root, v));
        }
        if(strcmp(s, "delete") == 0)
        {
            int v;
            scanf("%d", &v);
            root = remove(root, v);
            //root = rem(root);
        }
        if(strcmp(s, "clear") == 0)
        {
            del(root);
            root = nullptr;
        }
        if(strcmp(s, "exit") == 0)
        {
            del(root);
            root = nullptr;
            break;
        }
    }
    return 0;
}
