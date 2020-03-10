#include <unistd.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cerrno>

#include <algorithm>
#include <thread>

#include <boost/container/small_vector.hpp>
#include <boost/timer/timer.hpp>

int bind_listen( const char *ip, unsigned short port )
{
    //创建套接字
    int serv_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    //将套接字和IP、端口绑定
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));  //每个字节都用0填充
    serv_addr.sin_family = AF_INET;  //使用IPv4地址
    serv_addr.sin_addr.s_addr = inet_addr(ip);  //具体的IP地址
    serv_addr.sin_port = htons(port);  //端口
    while ( -1 == bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))) {
        printf( "1秒后重试bind\n" ) ; 
        sleep(1) ;
    }   
    //进入监听状态，等待用户发起请求
    while (-1 == listen(serv_sock, 20)) {
        printf( "1秒后重试listen\n" ) ; 
        sleep(1) ;
    }   
    return serv_sock ;
}

char buff[4096] ;
boost::container::small_vector<int, 1024> connfds ;

int main()
{
    int listenfd = bind_listen( "127.0.0.1", 12345 ) ;
    int flag = 1 ; 
    fd_set fds ;
    struct timeval ts ;
    ts.tv_sec = 0 ;
    ts.tv_usec = 0 ;
    int maxfd = listenfd ;
    char on = 1 ;
    while( 1 ) {
        FD_ZERO(&fds) ;
        FD_SET( listenfd, &fds ) ;
        for ( auto fd : connfds ) {
            //printf( "add connfd %d size %lu \n", fd, connfds.size() ) ;
            FD_SET( fd, &fds ) ;
        }
        int rc = select( maxfd + 1, &fds, NULL, NULL, &ts ) ;
        if ( -1 == rc ) {
            if ( EINTR == errno ) {
                continue ;
            }
            perror( "select" ) ;
            break ;
        }
        else if (0 == rc){
            //printf( "continue rc %d\n", rc ) ;
            continue ;
        }
        //printf( "rc %d\n", rc ) ;
        if ( FD_ISSET(listenfd, &fds) ) {
            int fd = accept( listenfd, NULL, NULL ) ;
            if ( fd > maxfd ) maxfd = fd ;
            if (-1 == setsockopt( fd, IPPROTO_TCP, TCP_NODELAY, &flag, 4 ) ) { 
                perror( "setsockopt" ) ; 
                return -1 ;
            }
            int nRecvBufLen = 0 ;
            setsockopt( fd, SOL_SOCKET, SO_RCVBUF, &nRecvBufLen, sizeof( int ) );
            setsockopt( fd, SOL_SOCKET, SO_SNDBUF, &nRecvBufLen, sizeof( int ) );
            connfds.push_back( fd ) ;
        }
        for ( auto fd : connfds ) {
            if ( FD_ISSET(fd, &fds) ) {
                //printf( "有新消息\n" ) ;
                //boost::timer::auto_cpu_timer t ;
                setsockopt(fd, IPPROTO_TCP, TCP_QUICKACK, &on, sizeof(on));
                int rbyte = recv( fd, buff, sizeof(buff), 0 ) ;
                if ( 0 == rbyte ) {
                    close(fd) ;
                    auto iter = std::find_if(connfds.begin(), connfds.end(), [fd](int findfd){
                            return fd == findfd ;}) ;
                    connfds.erase(iter) ;
                    printf( "客户端退出\n" ) ;
                }
                send( fd, buff, rbyte, 0 ) ;
            }
        }
    }
}
