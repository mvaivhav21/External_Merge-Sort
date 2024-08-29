#include<iostream>
#include<fstream>
#include<bits/stdc++.h>
#include<time.h>
typedef unsigned long long int ll;

using namespace std;

int main()
{
    ofstream fout; //defining output stream
    fout.open("input-file.txt",ios :: out); //Opening file

    time_t t; //Obtain system time 
    unsigned int seedval=(unsigned) time (&t); //Initialize time variable t with system time
    srand(seedval); //seed random number generator

    vector<int> data; //Define a vector which stores all the values that has to be written into the file
    // Assuming the keys are integers

    ll number_of_keys; //total number of keys, n

    cout<<"Enter the number of keys to be inserted into file: ";
    cin>>number_of_keys;

    for(ll i=0; i<number_of_keys; i++)
    {
       fout<<rand()<<"\n"; //Store random values into the output file
    }

    fout.close();//close file
    cout<<"Data written into file!\n";
    cout<<"Total Number of Keys: "<<number_of_keys;
    cout<<"\nSize of Each Key: "<<sizeof(int);
    return 0;
}
