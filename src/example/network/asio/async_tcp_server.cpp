#include <iostream>
#include <vector>
#include <functional>
#include <memory>
#include <chrono>
using namespace std ;

#include <boost/asio.hpp>
#include <boost/exception/exception.hpp>
#include <boost/exception/diagnostic_information.hpp>
using namespace boost ;
using namespace boost::asio ;
using namespace boost::asio::ip ;


char buffer_req[4096] ;
char buffer_rsp[4096] ;
io_service context ;
tcp::acceptor acceptor( context, tcp::endpoint(tcp::v4(), 12345) ) ;

vector<std::shared_ptr<tcp::socket>> sockClient_arr ;
tcp::no_delay option(true) ;

//void read_handle( tcp::socket &sockClient, const boost::system::error_code &ec, size_t s )
//{
//    sockClient.write( buffer(buffer_req, s) ) ;
//    sockClient.async_read_some( buffer(buffer_req), boost::bind(read_handle, sockClient, _1, _2) ) ;
//}

void read_handle( tcp::socket &sockClient, const boost::system::error_code &ec, size_t s )
{
    if ( ec ) {
        cout << "read error!\n" ;
        return ;
    }
    sockClient.write_some( buffer(buffer_req, s) ) ;
    sockClient.async_read_some( buffer(buffer_req), std::bind(read_handle, std::ref(sockClient),
                std::placeholders::_1, std::placeholders::_2) ) ;
}

//boost::asio::basic_waitable_timer<std::chrono::steady_clock> deadline_;
//
void second_1(boost::asio::basic_waitable_timer<std::chrono::steady_clock> &deadline, tcp::socket &sockClient)
{
    deadline.async_wait([&deadline, &sockClient](const boost::system::error_code &ec)mutable{
            if (ec) {
                cout << ec << "\n";
                return;
            }
            deadline = boost::asio::basic_waitable_timer<std::chrono::steady_clock>{sockClient.get_executor().context(), std::chrono::seconds(1)};
            second_1(deadline, sockClient);
            cout << "调用deadline\n";
            });

}

void accept_handle( tcp::socket &sockClient, const boost::system::error_code &ec )
{
    if ( ec ) {
        cout << "accept error !\n" ;
        return ;
    }
    auto ep = sockClient.remote_endpoint() ;
    cout << ep.address() << ":" << ep.port() << "\n" ;
    sockClient.set_option( option ) ;
    sockClient.async_read_some( buffer(buffer_req), std::bind(read_handle, std::ref(sockClient),
                std::placeholders::_1, std::placeholders::_2) ) ;
    static boost::asio::basic_waitable_timer<std::chrono::steady_clock> deadline{sockClient.get_executor().context(), std::chrono::seconds(1)};
    second_1(deadline, sockClient);



    sockClient_arr.emplace_back( new tcp::socket(context) ) ;
    acceptor.async_accept( *sockClient_arr.back(), std::bind(accept_handle, std::ref(*sockClient_arr.back()), 
                std::placeholders::_1) ) ;
}

int main()
{
    sockClient_arr.emplace_back( new tcp::socket(context) ) ;
    acceptor.async_accept( *sockClient_arr.back(), std::bind(accept_handle, std::ref(*sockClient_arr.back()), 
                std::placeholders::_1) ) ;
    while(1){
        context.poll();
    }
    context.run();
}
