#include <iostream>
#include <boost/timer/timer.hpp>
#include <boost/signals2.hpp>
using namespace std ;

void hello()
{
   //cout << "Hello World!\n" ;
}

int main()
{
    int64_t a;
    using namespace boost::signals2 ;
    using namespace boost::timer ;
    signal<void()> sig ;
    sig.connect(hello) ;
    auto_cpu_timer t ;
    hello() ;
    //sig() ;
    //cout << t.format() << "\n" ;
}
