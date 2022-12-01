/*
        Author: Jan Jurkowski
        Under GPLv3
        Example of Knuth-Morris-Pratt algorithm
                    */

#include<cstdio>
#include<cstring>

const int max_n = 1000000;

int pi[max_n];
char s[max_n];

bool kmp(int n, char* slowo)
{
    pi[0] = 0;
    int t = 0;
    for(int i = 1; i < n; i++)
    {
        while(slowo[i] != slowo[t] && t > 0)
            t = pi[t];
        if(slowo[i] == slowo[t])
            t++;
        pi[i] = t;
        //printf("%d %d %c %c\n", i, t, slowo[i], slowo[t]);
    }
    t = 0;
    int counter = 0;
    int n2 = strlen(slowo);
    for(int i = 0; i < n2; i++)
    {
        while(slowo[i] != slowo[t] && t > 0)
            t = pi[t];
        if(slowo[i] == slowo[t])
            t++;
        //printf("%d %d %d\n", i, t, counter);
        if(t == n)
        {
            t = pi[t - 1];
            if(counter >= n)
                return false;
            counter = -1;
        }
        counter++;
    }
    if(counter > 0)
        return false;
    return true;
}

int main()
{
    scanf("%s", s);
    kmp(strlen(s), s);
    int l = 1, p = strlen(s);
    while(p - l > 1)
    {
        int sr = (l + p) / 2;
        printf("%d %d %d %d\n", l, p, sr, kmp(sr, s));
        if(kmp(sr, s))
            p = sr;
        else
            l = sr;
    }
    int n = l;
    if(!kmp(l, s))
        n = p;
    for(int i = 0; i < n; i++)
        printf("%c", s[i]);
    return 0;
}
