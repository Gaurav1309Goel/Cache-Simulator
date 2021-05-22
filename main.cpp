//IMT2018009 Anish
//IMT2018025 Gaurav
//IMT2018039 Manan
//Direct mapped cache
#include <iostream>
#include <bits/stdc++.h>
#include <fstream>
using namespace std;
#define noffset 8
#define nindex 256
#define memsize 10000
typedef long long int ll;
/*
cache with size equal to nindex.
cache[i][0] stores dirty bit.
cache[i][1] stores valid bit.
cache[i][2] stores tag.
cache[i][3....noffset+3] stores a block of 16 bytes. 
*/

//function to convert string binary to integer.
ll binary_to_int(string binary)
{
    ll ans = 0;
    for (int i = 0; i < 32; i++)
    {
        if (binary[31 - i] == '1')
        {
            ans += (ll)pow(2, i);
        }
    }
    return ans;
}
//function to convert integer to string binary.
string int_to_binary(ll no)
{
    string t;
    ll counter = 0;
    while (no > 0)
    {
        t += no % 2 + 48;
        no /= 2;
        counter++;
    }
    while (counter < 32)
    {
        t += '0';
        counter++;
    }
    reverse(t.begin(), t.end());
    return t;
}
//main
int main()
{
    char comm;                                                  //load or store.
    ll addr;   //address in memory.
    ll tempaddr;
    ll tag;
    ll offset;
    ll no_of_instr;                                            //number of instructions between current memory access and last memory access.
    ll index;                                                  //index in cache.
    ll hits = 0;                                               //total number of hits.
    ll instr = 0;                                              //total number of instuctions.
    vector<vector<ll>> cache(nindex, vector<ll>(noffset + 3)); //cache
    ifstream file;
    file.open("mcf.trace"); //test file
    for (ll i = 0; i < nindex; i++)
    {
        cache[i][0] = 0; //intializing all dirty bits to 0.
        cache[i][1] = 0; //intializing all valid bits to 0.
    }

    while (!file.eof())
    {
        instr++;
        file >> comm;        //taking input for load or store
        file >> hex >> addr; //taking address.
        file >> no_of_instr;
        tempaddr = addr;
        offset = tempaddr % noffset; //extracting offset.
        tempaddr /= noffset;
        index = tempaddr % nindex; // extracting index
        tempaddr /= nindex;
        tag = tempaddr;           //extracting tag.
        ll hit = 0;               //checking if hit or miss.
                                  // cout << comm << " " << addr << " " << no_of_instr << "\n";
        if (cache[index][1] == 1) //checking valid bit
        {
            if (cache[index][2] == tag) //checking tag bit.
            {
                hit = 1;
                hits++;
            }
        }
        cache[index][1] = 1;   //making valid bit 1.
        cache[index][2] = tag; //changing tag.
        if (addr < memsize)    //checking addr less than memsize
        {
            //LOAD
            if (comm == 'l')
            {
                ll output;    //variable to store load value.
                if (hit == 1) //if hit is 1 directly taking from cache.
                {
                    output = cache[index][offset + 3];
                }
                else
                {
                    //if miss.
                    if (cache[index][0] == 1) //if dirty bit is 1 then we have to store the old data in the memory first.
                    {
                        ifstream memory;
                        ofstream mem1;
                        memory.open("example.txt");
                        mem1.open("example1.txt"); //creating temporary file for overwriting.
                        string temp;
                        ll cnt = 0;
                        /*joining tag and index bits*/
                        ll temp1 = cache[index][2] * nindex * noffset;
                        temp1 += index * noffset;
                        /* loop to traverse the memory.*/
                        for (int i = 0; i != temp1; i++)
                        {
                            memory >> temp;
                            mem1 << temp << endl;
                            cnt++;
                        }
                        //overwriting the 16 words of cache in memory
                        for (int i = 0; i < noffset; i++)
                        {
                            mem1 << int_to_binary(cache[index][i + 3]) << endl;
                            memory >> temp;
                            cnt++;
                        }
                        // writing remaning file.
                        for (int i = cnt; i < memsize; i++)
                        {
                            memory >> temp;
                            mem1 << temp << endl;
                        }
                        memory.close();
                        mem1.close();
                        remove("example.txt");
                        rename("example1.txt", "example.txt"); //renaming example1 to example.
                    }
                    /*writing the 16 miss words to cache*/
                    ifstream memory;
                    memory.open("example.txt");
                    string temp;
                    cache[index][0] = 0;
                    for (int i = 0; i != addr - offset; i++)
                    {
                        memory >> temp;
                    }
                    for (int i = 0; i < noffset; i++)
                    {
                        memory >> temp;
                        cache[index][i + 3] = binary_to_int(temp);
                    }
                    output = cache[index][offset + 3];
                    memory.close();
                }
                cout << output << "\n";
            }
            //STORE
            else
            {
                if (hit == 1)
                {
                    cache[index][offset + 3] = no_of_instr;
                    //updating directly to the cache if its a hit.
                }
                else
                {
                    //if miss
                    if (cache[index][0] == 1)
                    {
                        /*same as load first writing already exisitng data in memory if 
                        dirty bit is 1
                        */
                        ll cnt = 0;
                        ifstream memory;
                        ofstream mem1;
                        mem1.open("example1.txt");
                        memory.open("example.txt");
                        string temp;
                        ll temp1 = cache[index][2] * nindex * noffset;
                        temp1 += index * noffset;
                        for (int i = 0; i != temp1; i++)
                        {
                            memory >> temp;
                            mem1 << temp << endl;
                            cnt++;
                        }
                        for (int i = 0; i < noffset; i++)
                        {
                            mem1 << int_to_binary(cache[index][i + 3]) << endl;
                            memory >> temp;
                            cnt++;
                        }
                        for (int i = cnt; i < memsize; i++)
                        {
                            memory >> temp;
                            mem1 << temp << endl;
                        }
                        memory.close();
                        mem1.close();
                        remove("example.txt");
                        rename("example1.txt", "example.txt");
                    }
                    //writing the 16 miss words to cache
                    ifstream memory;
                    string temp;
                    memory.open("example.txt");
                    cache[index][0] = 1;
                    for (int i = 0; i != addr - offset; i++)
                    {
                        memory >> temp;
                    }
                    for (int i = 0; i < noffset; i++)
                    {
                        memory >> temp;
                        cache[index][i + 3] = binary_to_int(temp);
                    }
                    //updating the particular word in cache line to given data.
                    cache[index][3 + offset] = no_of_instr;
                    memory.close();
                }
            }
        }
    }
    //displaying hits and hit ratio.
    cout << "hits=" << hits << "\n";
    cout << "hit ratio=" << hits * (1.0 / instr) << "\n";
    return 0;
}