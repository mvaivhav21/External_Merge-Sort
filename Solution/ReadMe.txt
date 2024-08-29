heap.hpp is header file included for Heap data structure and related functions.

FinalSorted file contains the final sorted list of keys

Size of key is assumed to be 4 , instead of the value given in problem statement, since an integer ,on this system, occupies 4 bytes. In case it occupies 8 bytes on any other system, the size could be obtained via the output of MakeFile.cpp.

MakeFile.cpp contains the program to generate a given number of keys. All keys are assumed to be integers, and are written on different lines. 
Input: Number of Keys
Output: Size of Each Key; Data is written into input-file.txt

Runs folder contains all the various runs. The names of files are indexed via the output of MainProgram.cpp

MainProgram.cpp: 

Input: File containing the keys, size of available memory in number of blocks, size of each key in bytes, total number of keys, disk block size.
Output: Total Number of disk seeks and disk transfers, the number of merge passes, the number 9of disk seeks and disk transfers for the initial sorted run phase and each subsequent merge pass phase. The Final Sorted data is in FinaSorted.txt. Data of each run is stored within another folder "runs".

Note that after every execution of program, all the files from "runs" shpuld be cleared off.
Further, the "runs" folder should not be deleted.

The sample run(without time stamp) is shown below:

PS C:\Users\Mohank20\Desktop\Assignment1\Assignment1> g++ MakeFile.cpp
PS C:\Users\Mohank20\Desktop\Assignment1\Assignment1> .\a
Enter the number of keys to be inserted into file: 100000
Data written into file!
Total Number of Keys: 100000
Size of Each Key: 4
PS C:\Users\Mohank20\Desktop\Assignment1\Assignment1> g++ MainProgram.cpp
PS C:\Users\Mohank20\Desktop\Assignment1\Assignment1> .\a input-file.txt 10 4 100000 1024

Number of Disk Transfers in Initial Sorted Run Phase: 782
Number of Disk Seeks in Initial Sorted Run Phase: 80
First File Opened for Merging Arrays in current pass is: run1.txt
Last File Opened for Merging Arrays in current pass is: run40.txt

Current Pass is: 1
First File Opened for Merging Arrays in current pass is: run41.txt
Final File Opened for Merging Arrays in current pass is: run45.txt
Total Number of Disk Transfers after Current Merge Phase: 1564
Total Number of Disk Seeks after Current Merge Phase: 862

Current Pass is: 2
First File Opened for Merging Arrays in current pass is: run46.txt
Final File Opened for Merging Arrays in current pass is: run46.txt
Total Number of Disk Transfers after Current Merge Phase: 2346
Total Number of Disk Seeks after Current Merge Phase: 1644

Total Number of Disk Transfers: 2346
Total Number of Disk Seeks: 1644
Total Number Of Merge Passes: 2
PS C:\Users\Mohank20\Desktop\Assignment1\Assignment1> 