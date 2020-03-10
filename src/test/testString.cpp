#include <unistd.h>

#include <iostream>
#include <string>

#include <boost/timer/timer.hpp>
#include <boost/container/string.hpp>
#include <boost/lexical_cast.hpp>

#define COUNT 100000
int main()
{
    printf( "%u\n", getpid() ) ;
    char *buf = new char[1000] ;
    {
        boost::timer::auto_cpu_timer t ;
        for ( int i = 0; i < COUNT; ++i ) {
            strcpy( buf, "abcdefdkowpfjeef" ) ;
            strcat( buf, "12394023u43-24-138432" ) ;
            strcat( buf, "12394023u43-24-138432" ) ;
            strcat( buf, "12394023u43-24-138432" ) ;
            strcat( buf, "12394023u43-24-138432" ) ;
            strcat( buf, "12394023u43-24-138432" ) ;
            strcat( buf, "12394023u43-24-138432" ) ;
            strcat( buf, "12394023u43-24-138432" ) ;
            strcat( buf, "12394023u43-24-138432" ) ;
            strcat( buf, "12394023u43-24-138432" ) ;
            strcat( buf, "12394023u43-24-138432" ) ;
            strcat( buf, "12394023u43-24-138432" ) ;
            strcat( buf, "12394023u43-24-138432" ) ;
            strcat( buf, "12394023u43-24-138432" ) ;
            strcat( buf, "12394023u43-24-138432" ) ;
            strcat( buf, "12394023u43-24-138432" ) ;
            strcat( buf, "12394023u43-24-138432" ) ;
        }
    }
    {
        boost::timer::auto_cpu_timer t ;
        boost::container::string str ;
        str.resize(1000) ;
        for ( int i = 0; i < COUNT; ++i ) {
            str = "abcdefdkowpfjeef" ;
            str += "12394023u43-24-138432" ;
            str += "12394023u43-24-138432" ;
            str += "12394023u43-24-138432" ;
            str += "12394023u43-24-138432" ;
            str += "12394023u43-24-138432" ;
            str += "12394023u43-24-138432" ;
            str += "12394023u43-24-138432" ;
            str += "12394023u43-24-138432" ;
            str += "12394023u43-24-138432" ;
            str += "12394023u43-24-138432" ;
            str += "12394023u43-24-138432" ;
            str += "12394023u43-24-138432" ;
            str += "12394023u43-24-138432" ;
            str += "12394023u43-24-138432" ;
            str += "12394023u43-24-138432" ;
            str += "12394023u43-24-138432" ;
        }
    }
    {
        printf( "strcpy:" ) ;
        boost::timer::auto_cpu_timer t ;
        char dst[16], src[16] = "iiiiiiiiidowpje" ;
        for ( int i = 0; i < 100000000; ++i ) {
            strcpy( dst, src) ;
        }
        //printf( "dst %s\n", dst ) ;
    }
    {
        printf( "strlen:" ) ;
        boost::timer::auto_cpu_timer t ;
        char dst[16], src[16] = "iiiiiiiiidowpje" ;
        for ( int i = 0; i < 100000000; ++i ) {
            //memset( dst, 0, sizeof(dst) ) ;
            memcpy( dst, src, strlen(src) ) ;
        }
        //printf( "dst %s\n", dst ) ;
    }
    {
        printf( "memcpy:" ) ;
        boost::timer::auto_cpu_timer t ;
        char dst[16], src[16] = "iiiiiiiiidowpje" ;
        for ( int i = 0; i < 100000000; ++i ) {
            memcpy( dst, src, sizeof(dst) ) ;
        }
        //printf( "dst %s\n", dst ) ;
    }
    {
        printf( "int:" ) ;
        boost::timer::auto_cpu_timer t ;
        char dst[16], src[16] = "iiiiiiiiidowpje" ;
        for ( int i = 0; i < 100000000; ++i ) {
            *(__int128*)dst= *(__int128*)src ;
        }
        //printf( "dst %s\n", dst ) ;
    }
    {
        boost::timer::auto_cpu_timer t ;
        for ( int i = 0; i < 1000000; ++i ) {
            auto d = std::stod("123.4567890") ;
        }
    }
    {
        boost::timer::auto_cpu_timer t ;
        for ( int i = 0; i < 1000000; ++i ) {
            auto d = boost::lexical_cast<double>("123.4567890") ;
        }
        //printf( "dst %s\n", dst ) ;
    }
    
}
