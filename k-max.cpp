/*
        Author: Jan Jurkowski
        Under GPLv3
        Example of implementation of k-max priority
        queue, allows to insert, pop and get maximum value
        in constant time and linear memory.
                    */

#include<cstdio>
#include<list>

const int inf = 1<<30;

typedef std::list<std::pair<int, int> > lista;

int maks(lista &queue)
{
    if(!queue.empty())
        return queue.front().first;
    else
        return -inf;
}

bool pop(lista &queue)
{
    queue.front().second--;
    if(queue.front().second == 0)
    {
        queue.pop_front();
        return true;
    }
    return false;
}

void push(lista &queue, int v)
{
    int suma = 0;
    while(!queue.empty() && queue.back().first < v)
    {
        suma += queue.back().second;
        queue.pop_back();
    }
    queue.push_back(std::make_pair(v, suma + 1));
}

int liczby[1000000];

int main()
{
    lista queue;
    int n;
    int k;
    scanf("%d %d", &n, &k);
    for(int i = 0; i < n; i++)
    {
        scanf("%d", &liczby[i]);
    }
    for(int i = 0; i < n; i++)
    {
        if(i >= k)
            pop(queue);
        push(queue, liczby[i]);
        printf("%d ", maks(queue));
    }
    return 0;
}
