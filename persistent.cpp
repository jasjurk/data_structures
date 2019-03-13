/*
        Author: Jan Jurkowski
        Under GPLv3
        Example of implementation of persistent heap
        Uses O(n) space and O(log n) time on all operations
                    */

#include<cstdio>
#include<algorithm>
#include<cstring>

class node
{
public:
    int value = 0;
    int size = 1;
    node* left = nullptr;
    node* right = nullptr;

public:
    node(int _value)
    {
        value = _value;
    }
};

namespace pers
{

int value(node* pointer)
{
    if(pointer == nullptr)
        return ~(1<<30);
    else
        return pointer->value;
}

int size(node* pointer)
{
    if(pointer == nullptr)
        return 0;
    else
        return pointer->size;
}

node* insert(node* pointer, int v)
{
    if(pointer == nullptr)
        return new node(v);
    pointer = new node(*pointer);
    if(v > pointer->value)
        std::swap(v, pointer->value);
    if(size(pointer->left) < size(pointer->right))
    {
        pointer->left = insert(pointer->left, v);
        pointer->size += size(pointer->left);
    }
    else
    {
        pointer->right = insert(pointer->right, v);
        pointer->size += size(pointer->right);
    }
}

node* pop(node* pointer)
{
    if(pointer == nullptr)
        return nullptr;
    if(pointer->left == nullptr && pointer->right == nullptr)
        return nullptr;
    pointer = new node(*pointer);
    if(value(pointer->left) >= value(pointer->right))
    {
        std::swap(pointer->value, pointer->left->value);
        pointer->left = pop(pointer->left);
    }
    else
    {
        std::swap(pointer->value, pointer->right->value);
        pointer->right = pop(pointer->right);
    }
    return pointer;
}

void cleanup(node* pointer)
{
    if(pointer == nullptr)
        return;
    cleanup(pointer->left);
    cleanup(pointer->right);
}
}

class heap
{
    std::vector<node*> roots;

private:
    node* back()
    {
        return roots[roots.size() - 1];
    }

public:
    heap()
    {
        roots.push_back(nullptr);
    }

    void insert(int v)
    {
        roots.push_back(pers::insert(back(), v));
    }

    int top()
    {
        return pers::value(back());
    }

    int pop()
    {
        int v = pers::value(back());
        roots.push_back(pers::pop(back()));
        return v;
    }

    void restore(int i)
    {
        roots.push_back(roots[i]);
    }
};

heap kop;

int main()
{
    char command[256];
    int i = 0;
    while(true)
    {
        i++;
        scanf("%s", &command);
        if(strcmp(command, "insert") == 0)
        {
            int a;
            scanf("%d", &a);
            kop.insert(a);
        }
        if(strcmp(command, "pop") == 0)
        {
            printf("%d\n", kop.pop());
        }
        if(strcmp(command, "top") == 0)
        {
            printf("%d\n", kop.top());
        }
        if(strcmp(command, "restore") == 0)
        {
            int a;
            scanf("%d", &a);
            kop.restore(a);
        }
        if(strcmp(command, "exit") == 0)
        {
            /*for(int i2 = 0; i2 < i; i2++)
                cleanup(roots[i2]);*/
            return 0;
        }
        //printf("%d\n", value(roots[i]));
    }
    return 0;
}
