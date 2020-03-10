#include <iostream>
using namespace std ;

#include <boost/circular_buffer.hpp>
using namespace boost ;

template <typename T>
ostream &operator<<( ostream &os, const circular_buffer<T> &cb )
{
    for ( auto &t : cb ) {
        os << t << ' ' ;
    }
    return os ;
}

int main()
{
    circular_buffer<int> cb(3) ;
    cb.push_back(1) ;
    cb.push_back(2) ;
    cb.push_back(3) ;
    cout << cb << "\n" ;

    cb.push_back(4) ;
    cb.push_back(5) ;
    cout << cb << "\n" ;
    cb.pop_front() ;
    cout << cb << "\n" ;
    cb.pop_back() ;
    cout << cb << "\n" ;
}
