#include <unistd.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <cassert>
#include <cmath>

#include <iostream>
using namespace std ;

#include <boost/timer/timer.hpp>

char on = 1 ;
int connectServer( const char *ip, int port )
{
    int connectfd = socket( AF_INET, SOCK_STREAM, 0 ) ;
    assert( -1 != connectfd ) ;
    //向服务器（特定的IP和端口）发起请求
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));  //每个字节都用0填充
    serv_addr.sin_family = AF_INET;  //使用IPv4地址
    serv_addr.sin_addr.s_addr = inet_addr(ip);  //具体的IP地址
    serv_addr.sin_port = htons(port);  //端口
    if ( -1 == connect(connectfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) ) {
        return -1 ;
    }
    static int flag = 1 ;
    if (-1 == setsockopt( connectfd, IPPROTO_TCP, TCP_NODELAY, &flag, 4 ) ) {
        perror( "setsockopt" ) ;
        return -1 ;
    }
    int nRecvBufLen = 0; //设置为0
    setsockopt( connectfd, SOL_SOCKET, SO_RCVBUF, &nRecvBufLen, sizeof( int ) );
    setsockopt( connectfd, SOL_SOCKET, SO_SNDBUF, &nRecvBufLen, sizeof( int ) );
    setsockopt( connectfd, IPPROTO_TCP, TCP_QUICKACK, &on, sizeof(on) );
    return connectfd ;
}

inline int recv_loop( int fd, void *buf, size_t len )
{
    int rc = -1 ;
    while ( 0 >= rc ) {
        rc =::recv( fd, buf, len, MSG_DONTWAIT ) ;
        if ( -1 == rc ) {
            setsockopt( fd, IPPROTO_TCP, TCP_QUICKACK, &on, sizeof(on) );
            continue ;
        }
        //if ( (-1 == rc && (EAGAIN == errno || EINTR == errno || EWOULDBLOCK == errno)) ) continue ;
        if ( !rc ) {
            printf( "rc %d errno %d\n", rc, errno ) ;
            return -1 ;
        }
        return rc ;
    }
    return rc ;
}

char sndbuf[4096] ;
char rcvbuf[4096] ;
int main()
{
    int connfd = connectServer( "192.168.1.6", 12345 ) ;
    while ( 1 ) {
        //cout << "请输入:" ;
        //cin >> sndbuf ;
        strcpy( sndbuf, "12313122313123" ) ;
        int wbyte = strlen(sndbuf) ;
        memset(rcvbuf, 0, sizeof(rcvbuf)) ;
        {
            boost::timer::auto_cpu_timer t ;
            recv_loop( connfd, rcvbuf, send( connfd, sndbuf, wbyte, 0 )) ;
        }
        setsockopt( connfd, IPPROTO_TCP, TCP_QUICKACK, &on, sizeof(on) );
        //printf( "%s\n", rcvbuf ) ;
        //sleep(1) ;


        //for ( int i = 0; i < 1000000000; ++i ) {
        //    sqrt( i ) ;
        //}
    }
}
