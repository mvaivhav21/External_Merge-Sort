#include <vector>
using namespace std;
// Template class to include both integer type and pair of integers

template<class T>
class heap {
    public:
    vector<T> h; //heap
    T err;//error
    T inf;//infinity

    heap(T err,T inf);
    void heapify(int i);
    T min();
    T get(int i);
    T delete_min();
    void insert(T x);
    T parent(int x);
    T left(int x);
    T right(int x);
    int size();
    void decrease_key(int i,T y);
    void clear();
};

/// Constructor 
template<class T>
heap<T>::heap(T er,T in) {
    err=er; inf=in;
    return;
}

/// Returns size of the heap()
template<class T>
int heap<T>::size() {
    return h.size();
}

/// Gives the parent of a node of it exists
/// otherwise -1.
template<class T>
T heap<T>::parent(int x) {
    int t = (x%2==0) ? ((x/2)-1) : (x/2) ;
    if(t<0) return err;
    else return h[t];
}

/// Gives the left child of a node of it exists
/// otherwise -1.
template<class T> 
T heap<T>::left(int x) {
    if((2*x)>=h.size()) return -1;
    return h[2*x+1];
}

/// Gives the rigth child of a node of it exists
/// otherwise -1.
template<class T>
T heap<T>::right(int x) {
    if((2*x+1)>=h.size()) return -1;
    return h[2*x+2];
}

/// At a particular node on the heap if
/// the subtrees are already heaps but the root is displaced
/// then this function will make a heap()
template<class T>
void heap<T>::heapify(int i) {
    int t;
    if((2*i+1)<size() && h[2*i+1]<h[i]) t=2*i+1;
    else t=i;
    if((2*i+2)<size() && h[2*i+2]<h[t]) t=2*i+2;
    if(i!=t) {
        swap(h[i],h[t]);
        heapify(t);
    }
    return;
}

/// Gives minimum element in the heap
template<class T>
T heap<T>::min() {
    return h[0];
}

/// Deletes the minimum element from the heap.
template<class T>
T heap<T>::delete_min() {
    swap(h[0],h[h.size()-1]);
    T p=h[h.size()-1];
    h.pop_back(); heapify(0);
    return p;
}

/// Decrease the given element to a particular value and than
/// rebuild the heap.
template<class T>
void heap<T>::decrease_key(int i,T y) {
    h[i]=y;
    while(i>0 && h[i]<parent(i)) {
        int t;
        if(i%2==1) t=i/2;
        else t=(i/2)-1;
        swap(h[i],h[t]);
        i=t;
    }
    return;
}

/// Insert a particular value to the heap.
template<class T>
void heap<T>::insert(T x) {
    h.push_back(inf);
    if(h[h.size()-1]>x) decrease_key(h.size()-1,x);
    else h[h.size()-1]=x;
    return;    
}

/// Clear the Heap for new data
template<class T>
void heap<T>::clear() {
    h.clear();
    return;
}

/// Takes a arbitrary array and returns a heap object made from
/// that array elements.
template<typename T>
heap<T> build_heap(vector<T> a,int n,T err,T inf) {
    heap<T> hp(err,inf);
    for(int i=0;i<n;i++) hp.h.push_back(a[i]);
    int m=hp.size()/2;
    for(int i=m;i>-1;i--) hp.heapify(i);
    return hp;
}

/// Sorts a array using heaps in the background.
template<typename T>
vector<T> heap_sort(vector<T> a,T err,T inf) {
    heap<T> h=build_heap(a,a.size(),err,inf);
    vector<T> arr(a.size());
    for(int i=0;i<a.size();i++) {
        arr[i]=h.delete_min();
    }
    return arr;
}
