#include <iostream>
using namespace std ;

#include <boost/asio.hpp>
#include <boost/exception/exception.hpp>
#include <boost/exception/diagnostic_information.hpp>
using namespace boost ;
using namespace boost::asio ;
using namespace boost::asio::ip ;


char buffer_rep[4096] ;
char buffer_rsp[4096] ;

int main()
{
    io_service context ;
    tcp::acceptor acceptor( context, tcp::endpoint(tcp::v4(), 12345) ) ;
    tcp::socket sockClient( context ) ;
    while ( 1 ) {
        cout << "等待新连接\n" ;
        acceptor.accept( sockClient ) ;
        auto ep = sockClient.remote_endpoint() ;
        cout << ep.address() << ":" << ep.port() << "\n" ;
        while ( 1 ) {
            try {
                auto rbyte = sockClient.read_some( buffer(buffer_rep) ) ;
                sockClient.write_some( buffer(buffer_rep, rbyte) ) ;
            }
            catch ( const boost::exception &e ) {
                cout << diagnostic_information(e) << endl ;
                sockClient.close() ;
                break ;
            }
        }
    }
}
