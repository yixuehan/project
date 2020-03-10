#include "src/example/string/switch_string.hpp"
int main()
{
    boost::timer::auto_cpu_timer t ;
    for( int i = 0; i < 100000000; ++i ) {
        IntStr( "abcdefdo" ) ;
    }

    switch ( IntStr("1234567812345678") ) {
    case IntStr("1234567812345678"):
        break ;
    case IntStr("1234567812345679"):
        break ;
    case IntStr("123456781234567"):
        break ;
    case IntStr("abc2"):
        break ;
    case IntStr("abc3"):
        break ;
    case IntStr("abc4"):
        break ;
    case IntStr("abc5"):
        break ;
    }
}
