#include <iostream>
#include <bits/stdc++.h>
#include <fstream>
#include <time.h>
#define memsize 10000
using namespace std;
typedef long long int ll;
int main()
{
    ofstream file;
    file.open("example.txt");
    srand(time(0));
    for (ll i = 0; i < memsize; i++)
    {
        ll temp = rand() % 10000000;
        ll counter = 0;
        while (temp > 0)
        {
            counter++;
            ll temp1 = temp % 2;
            file << temp1;
            temp /= 2;
        }
        while (counter < 32)
        {
            counter++;
            file << 0;
        }
        file << "\n";
    }
    file.close();
    return 0;
}