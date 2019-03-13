/*
        Author: Jan Jurkowski
        Under GPLv3
        This is simple implementation of Fibonacci heap,
        a priority queue which has a constant insert, meld,
        key decrease times and a logarithmic time of deletion
                    */

#ifndef FIBONACCI
#define FIBONACCI
#include<cstdio>

typedef unsigned char byte;

struct node
{
    int value = 0;
    byte degree = 0;
    node* parent = this;
    node* child = nullptr;
    node* next = this;
    node* prev = this;
    node(int v) {value = v;}
};

int degree(node* pointer) {return pointer->degree & ~(1<<8);}

bool marked(node* pointer) {return (pointer->degree & (1<<8)) != 0;}

namespace dl_list
{
    node* insert(node* prev, int v)
    {
        if(prev == nullptr)
        {
            prev = new node(v);
            return prev;
        }
        node* pointer = new node(v);
        node* next = prev->next;
        next->prev = pointer;
        prev->next = pointer;
        pointer->next = next;
        pointer->prev = prev;
        prev = pointer;
        return pointer;
    }

    node* remove(node* &pointer)
    {
        if(pointer->next == pointer)
        {
            delete pointer;
            pointer = nullptr;
            return pointer;
        }
        node* next = pointer->next;
        node* prev = pointer->prev;
        next->prev = prev;
        prev->next = next;
        delete pointer;
        pointer = prev;
        return prev;
    }

    node* cfl(node* pointer)
    {
        if(pointer->next == pointer)
            return nullptr;
        node* next = pointer->next;
        node* prev = pointer->prev;
        next->prev = prev;
        prev->next = next;
        pointer->next = pointer;
        pointer->prev = pointer;
        return prev;
    }

    node* operator+(node p, int n) {node* pointer = &p;for(int i = 0; i < n; i++) pointer = pointer->next; return pointer;}

    void prev(node* &pointer) {pointer = pointer->prev;}

    void next(node* &pointer) {pointer = pointer->next;}

    node* meld(node* first, node* second)
    {
        if(second == nullptr)
            return first;
        if(first == nullptr)
            return second;
        node* fp = first->prev;
        node* sp = second->prev;
        first->prev = sp;
        sp->next = first;
        second->prev = fp;
        fp->next = second;
        return first;
    }
}

class heap
{
public:
    node* min = nullptr;

    int roots = 0;

    void consolidate()
    {
        using namespace dl_list;
        node* wyst[128] = {nullptr};
        node* pointer2 = min;
        for(int i = 0; i < roots; i++)
        {
            node* pointer = pointer2;
            next(pointer2);
            int d = degree(pointer);
            cfl(pointer);
            while(wyst[d] != nullptr)
            {
                if(pointer->value > wyst[d]->value)
                {
                    node* temp = pointer;
                    pointer = wyst[d];
                    wyst[d] = temp;
                }
                pointer->child = meld(pointer->child, wyst[d]);
                wyst[d] = nullptr;
                pointer->degree++;
                d = degree(pointer);
            }
            wyst[d] = pointer;
        }
        roots = 0;
        min = nullptr;
        for(int i = 0; i < 128; i++)
        {
            if(wyst[i] != nullptr)
                roots++;
            if(min != nullptr)
                meld(min, wyst[i]);
            else
                min = wyst[i];
            if(min != nullptr && wyst[i] != nullptr)
                if(min->value > wyst[i]->value)
                    min = wyst[i];
        }
    }

public:
    heap() {}

    void insert(int a)
    {
        roots++;
        if(min == nullptr || a < min->value)
            min = dl_list::insert(min, a);
        else
            dl_list::insert(min, a);
    }

    int top()
    {
        if(min == nullptr)
            return 1<<31 - 1;
        return min->value;
    }

    void pop()
    {
        using namespace dl_list;
        if(min == nullptr)
            return;
        node* pointer = min->child;
        if(pointer != nullptr)
        {
            do
            {
                roots++;
                pointer->parent = pointer;
                next(pointer);
            }while(pointer != min->child);
            meld(min, min->child);
        }
        roots--;
        min = remove(min);
        consolidate();
    }

    void print(node* parent)
    {
        node* pointer = parent->child;
        if(pointer == nullptr)
            return;
        do
        {
            printf("%d ", pointer->value);
            dl_list::next(pointer);
        }while(pointer != parent->child);
        printf("\n");
        pointer = parent->child;
        do
        {
            print(pointer);
            dl_list::next(pointer);
        }while(pointer != parent->child);
    }

    void print_roots()
    {
        if(min == nullptr)
        {
            printf("There's nothing in this heap\n");
            return;
        }
        node* pointer = min;
        do
        {
            printf("ROOT: %d\n", pointer->value);
            print(pointer);
            dl_list::next(pointer);
        }while(pointer != min);
    }
};

#endif
