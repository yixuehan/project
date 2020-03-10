#include <iostream>
using namespace std ;

#include <boost/asio.hpp>
#include <boost/timer/timer.hpp>
using namespace boost ;
using namespace boost::asio ;
using namespace boost::asio::ip ;


char buffer_rep[4096] ;
char buffer_rsp[4096] ;

int main()
{
    io_service context ;
    tcp::socket sockClient( context ) ;
    tcp::endpoint epServer(tcp::v4(), 12345) ;
    sockClient.connect( epServer ) ;
    tcp::no_delay option(true) ;
    sockClient.set_option( option ) ;
    auto ep = sockClient.local_endpoint() ;
    cout << ep.address() << ":" << ep.port() << "\n" ;
    while ( 1 ) {
        cout << "请输入:" ;
        cin >> buffer_rep ;
        auto wbyte = strlen(buffer_rep) ;
        {
            timer::auto_cpu_timer t ;
            sockClient.write_some( buffer(buffer_rep, wbyte) ) ;
            sockClient.receive( buffer(buffer_rep, wbyte) ) ;
        }
        cout << buffer_rep << "\n" ;
    }
}
