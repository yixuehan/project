#include <iostream>
using namespace std ;

#include <boost/asio.hpp>
#include <boost/timer/timer.hpp>
using namespace boost ;
using namespace boost::asio ;
using namespace boost::asio::ip ;


char buffer_req[4096] ;
char buffer_rsp[4096] ;

timer::cpu_timer t ;
io_service context ;
tcp::socket sockClient( context ) ;
tcp::endpoint epServer(tcp::v4(), 12345) ;

void send_buffer( const boost::system::error_code& ec, size_t s )
{
    cout << t.format() ;
    cout << "请输入:" ;
    cin >> buffer_req ;
    auto wbyte = strlen(buffer_req) ;
    t.start() ;
    sockClient.write_some( buffer(buffer_req, wbyte) ) ;
    sockClient.async_read_some( buffer(buffer_req, wbyte), send_buffer ) ;
}

int main()
{
    sockClient.async_connect( epServer, [](const boost::system::error_code &ec){
        auto ep = sockClient.local_endpoint() ;
        cout << ep.address() << ":" << ep.port() << "\n" ;
        cout << "请输入:" ;
        cin >> buffer_req ;
        auto wbyte = strlen(buffer_req) ;
        t.start() ;
        sockClient.set_option( tcp::no_delay(true) ) ;
        sockClient.async_write_some( buffer(buffer_req, wbyte), send_buffer ) ;
            }) ;

    //while ( context.run_one() ) ;
    context.run() ;
}
