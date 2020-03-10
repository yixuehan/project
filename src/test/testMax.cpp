#include <unistd.h>

#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <map>
using namespace std ;

#include <boost/timer/timer.hpp>
#include <boost/container/flat_map.hpp>

//#define FLAT
#ifdef UNORDERED
using Map = unordered_map<double, int>;
#elif defined(FLAT)
using Map = boost::container::flat_map<double, int>;
#else
using Map = map<double, int>;
#endif

int main()
{
    cout << getpid() << endl;
    {
        //boost::container::flat_map<double, int> m ;
        Map m ;
        boost::timer::auto_cpu_timer t ;
        Map::iterator iter;
        for ( int i = 0; i < 1000000; ++i ) {
            m[123.45] = 3 ;
            m[23.13] = 4 ;
            m[54.12] = 5 ;
            m[12.42] = 6 ;
            iter = max_element(m.begin(), m.end(), [](auto p1, auto p2){
                    return p1.second < p2.second ;
                    }
                    ) ;
            m.clear();
        }
        cout << iter->second << endl ;
    }
    //cout << iter->first << ' ' << iter->second << endl ;
}
