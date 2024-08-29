#include<bits/stdc++.h>
#include<time.h>
#include "heap.hpp"
typedef unsigned long long int ll;
ll KeyMaxVal=INT_MAX; //Maximum value of key possible
using namespace std;
#define inf INT_MAX

int min_val(int a,int b) {
    if(a>=b) return b;
    else return a;
}// returns smaller of two numbers

vector<int> read_block(ifstream& fin,int count,int& flag) {
    vector<int> arr;
    for(int BlockPointer=0; BlockPointer<count; BlockPointer++) {
        int ReadKey;
        if(fin.eof()) break;
        fin>>ReadKey;
        arr.push_back(ReadKey);
    }
    if(fin.eof()) flag=1;
    return arr;
} //reads a block from input stream , into a vector , and returns the vector

int main(int argc, char* argv[])
{
    clock_t start,end;
    start=clock();
    // Step 1 -- Take Inputs
    string inputfile;
    //cout<<"Enter the file name which contains the keys: ";
    inputfile=argv[1];

    vector<int> data;
    ll number_of_keys;
    ll MemoryBlockNumber;
    int KeySize;
    ll DiskBlockSize;
    int SizeOfKeysBytes;
    
    int NumberOfMergePasses=1;
    int NumberOfDiskTransfers=1;
    int NumberOfDiskSeeks=0;

    //cout<<"\nEnter size of available memory(in number of blocks): ";
    MemoryBlockNumber=atoi(argv[2]);
    //cout<<"\nEnter Size of each Key(in bytes): ";
    SizeOfKeysBytes=atoi(argv[3]);
    //cout<<"\nEnter Total Number of Keys: ";
    number_of_keys=atoi(argv[4]);
    //cout<<"Enter disk block size(in bytes)";
    DiskBlockSize=atoi(argv[5]);
    cout<<"\n";

    // Step 2 -- Make Sorted Runs
    ifstream fin(inputfile,ios::in);
    int NumberOfSortedRuns= ceil(double(number_of_keys*SizeOfKeysBytes)/double(DiskBlockSize*MemoryBlockNumber)); //Formula discussed in lectures
    //cout<<NumberOfSortedRuns<<"\n";

    for(int i=1;i<=NumberOfSortedRuns;i++ ) {

        string filename="runs/run"+to_string(i)+".txt"; // To define output file name
        ofstream srp(filename,ios::out);
        vector<int> SortedRunArray;

        for(int BlockPointer=0; BlockPointer<(MemoryBlockNumber*DiskBlockSize/SizeOfKeysBytes);BlockPointer++) {
            int ReadKey;
            if(fin.eof()) { SortedRunArray.pop_back(); break; }
            fin>>ReadKey;
            SortedRunArray.push_back(ReadKey);
        } //Read values block-by-block, and fill up all the m blocks of memory at a time, in order to sort it

        if(SortedRunArray.size()==0) continue;//If no elements in the memory
        
        vector<int> Sorted=heap_sort(SortedRunArray,-1,inf);//Sort the elements, neglecting extra memory required for sorting
        
        for(auto x:Sorted) srp<<x<<"\n"; //Output the fils into "sorting run phase" file
        srp<<"\n";

        NumberOfDiskTransfers+=SortedRunArray.size() *SizeOfKeysBytes / DiskBlockSize;
        NumberOfDiskSeeks++;
        //Increment seeks and transfers to n
        srp.close();
    }
    //cout<<"\n";
    fin.close();
    NumberOfDiskSeeks*=2;
    NumberOfDiskTransfers*=2;
    // Since number of disk seeks and disk transfers after initial sorted run = 2n

    cout<<"Number of Disk Transfers in Initial Sorted Run Phase: "<<NumberOfDiskTransfers<<"\n";
    cout<<"Number of Disk Seeks in Initial Sorted Run Phase: "<<NumberOfDiskSeeks<<"\n";
    cout<<"First File Opened for Merging Arrays in current pass is: run1.txt"<<"\n";
    cout<<"Last File Opened for Merging Arrays in current pass is: run"<<NumberOfSortedRuns<<".txt"<<"\n";

    // Make 3 - Merging of Arrays
    int current_count=NumberOfSortedRuns+1; // Current File Number
    int merged=0;                           // Files already merged
    while(NumberOfSortedRuns>1) { 
        int newNumberofSortedRuns=0;
        int file_pointer=merged+1;
        merged+=NumberOfSortedRuns;
        int iterate=ceil(double(NumberOfSortedRuns)/double(MemoryBlockNumber-1)); //Number of times, in one sorted run,m-1 blocks of data have to be copied into memory

            cout<<"\nCurrent Pass is: "<<NumberOfMergePasses;
            cout<<"\nFirst File Opened for Merging Arrays in current pass is: run"<<current_count<<".txt";

        // Merging of Files named as run<start>.txt to run<end>.txt
        while(iterate--) { 
            
            int check=min_val(MemoryBlockNumber-1,NumberOfSortedRuns); //If number of sorted runs is less than m-1, then we merge all the sorted runs in one go
            int start=file_pointer,end=file_pointer+check-1;//To mark start and end of reading values
            NumberOfSortedRuns-=check;
            int block_size=DiskBlockSize/SizeOfKeysBytes;
            file_pointer=end+1;//Increment file pointer to next read
            int counter=0;

            // Output File for merging the input files
            string output_file="runs/run";
            output_file=output_file+to_string(current_count)+".txt";

            current_count++; newNumberofSortedRuns++;
            ofstream fout(output_file,ios::out);
            
            // Input streams to be used for merging
            vector<ifstream> fin(check);
            vector<vector<int>> ArraysToBeMerged(check);
            vector<int> current_reading(check,1);
            vector<int> flag_read(check,0);
            heap<pair<int,int>> hp({-1,-1},{inf,inf});//Insert -1 and infinity

            for(int i=0;i<check;i++) {
                string fxn="runs/run"+to_string(start+i)+".txt";
                fin[i].open(fxn,ios::in);
                ArraysToBeMerged[i]=read_block(fin[i],block_size,flag_read[i]);
                if(flag_read[i]==1) counter++;
                hp.insert({ArraysToBeMerged[i][0],i});
            }//Insert first element of all the blocks to be merged into the heap

            int seekpointer=0;
            // part_sorted being filled once and used to output in stream.
            // We are using k-way merging using MinHeap, ignoring the memory required for Heap
            while(counter<check) {
                
                vector<int> part_sorted;
                for(int i=0;i<block_size && hp.size()!=0;i++) {
                    pair<int,int> p=hp.delete_min();
                    part_sorted.push_back(p.first);
                    
                    if(current_reading[p.second]==ArraysToBeMerged[p.second].size()) { //One block of disk has been read
                        vector<int> new_val=read_block(fin[p.second],block_size,flag_read[p.second]);
                        if(flag_read[p.second]==1) {
                            counter++; //One file has been read fully, now we would use one less number of blocks of memory
                            if(!new_val.empty()) new_val.pop_back();
                        }
                        ArraysToBeMerged[p.second]=new_val;
                        current_reading[p.second]=0;
                    }
                    if(ArraysToBeMerged[p.second].size()!=0) hp.insert({ArraysToBeMerged[p.second][current_reading[p.second]],p.second});
                    current_reading[p.second]++;//Increment pointer of that block which had minimum element
                    
                }
                
                //cout<<part_sorted.size()<<" ";
                for(auto it:part_sorted) fout<<it<<"\n";

                NumberOfDiskSeeks+=2;
                NumberOfDiskTransfers+= 2 * part_sorted.size() *SizeOfKeysBytes / DiskBlockSize;
            }
            
            // Closing all the files which were to be merged currently
            for(int i=0;i<check;i++) fin[i].close();
            fout.close();
        }
        
        // Updating the Current Number of Sorted Runs
        NumberOfSortedRuns=newNumberofSortedRuns;
        NumberOfMergePasses++;
        NumberOfDiskTransfers++;
        cout<<"\nFinal File Opened for Merging Arrays in current pass is: run"<<current_count-1<<".txt"<<"\n";
        cout<<"Total Number of Disk Transfers after Current Merge Phase: "<<NumberOfDiskTransfers<<"\n";
        cout<<"Total Number of Disk Seeks after Current Merge Phase: "<<NumberOfDiskSeeks<<"\n";
        
        // Making the final output file
        if(NumberOfSortedRuns==1) {
            int NumberOfEntries=0;

            ifstream xyz("Data.txt", ios::in);
            xyz.seekg(0, ios::end);
            int file_size = xyz.tellg(); 

            string OneKey,finaloutputfile="runs/run";
            finaloutputfile=finaloutputfile+to_string(current_count-1)+".txt";

            ifstream fin{finaloutputfile,ios::in};
            ofstream fout{"FinalSorted.txt",ios::out};

            if (fin && fout) {
                while (getline(fin, OneKey)) {
                    NumberOfEntries++;
                    if(NumberOfEntries<=number_of_keys) fout << OneKey << "\n";
                }
            }
            fin.close();
            fout.close();
        }
    }

    cout<<"\nTotal Number of Disk Transfers: "<<NumberOfDiskTransfers<<"\n";
    cout<<"Total Number of Disk Seeks: "<<NumberOfDiskSeeks<<"\n";
    cout<<"Total Number Of Merge Passes: "<<NumberOfMergePasses-1;
    end=clock();
    double time_taken=double(end - start) / double(CLOCKS_PER_SEC);
    cout << "\nTime taken by program is : " << time_taken << setprecision(5);
    cout << " sec " << endl;
    return 0;
}
