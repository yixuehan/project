#include <iostream>
#include <thread>
#include <atomic>
using namespace std;

//atomic<int> a;
//atomic<int> b;
int a = 0;
int b = 0;

int thread_1()
{
    int t = 1;
    a = t;
    b = 2;
    //cout << "b = " << b << endl;
    return 0;
}

int thread_2()
{
    while(b != 2){
    }
    cout << "th b = " << b << endl;
    cout << a << endl;
    if ( 1 != a ) {
        exit(1);
    }
    return 0;
}

int main()
{
    for (;;){
        printf( "next thread\n");
        thread t2(thread_2);
        thread t1(thread_1);
        t1.join();
        t2.join();
        a = 0;
        b = 0;
    }
}

