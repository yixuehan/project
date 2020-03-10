#include <unistd.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <cassert>

#include <iostream>
using namespace std ;

#include <boost/timer/timer.hpp>

int recv_loop( int fd, void *buf, size_t len )
{
    int rc = -1 ;
    while ( 0 >= rc ) {
        rc = read( fd, buf, len ) ;
        if ( -1 == rc ) continue ;
        //if ( (-1 == rc && (EAGAIN == errno || EINTR == errno || EWOULDBLOCK == errno)) ) continue ;
        if ( !rc ) {
            printf( "rc %d errno %d\n", rc, errno ) ;
            return -1 ;
        }
        return rc ;
    }
    return rc ;
}
inline int recv_loop( int fd, void *buf, size_t len, sockaddr *addr, socklen_t *plen )
{
    int rc = -1 ;
    while ( 0 >= rc ) {
        rc =::recvfrom( fd, buf, len, MSG_DONTWAIT, addr, plen ) ;
        if ( -1 == rc ) continue ;
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
    int serverfd = socket( AF_INET, SOCK_DGRAM, 0 ) ;
    struct sockaddr_in server_addr;
    socklen_t len = sizeof(server_addr) ;
    memset(&server_addr, 0, sizeof(server_addr));  //每个字节都用0填充
    server_addr.sin_family = AF_INET;  //使用IPv4地址
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");  //具体的IP地址
    server_addr.sin_port = htons(12345);  //端口

    struct sockaddr_in client_addr;
    memset(&client_addr, 0, sizeof(client_addr));  //每个字节都用0填充
    client_addr.sin_family = AF_INET;  //使用IPv4地址
    client_addr.sin_addr.s_addr = inet_addr("127.0.0.1");  //具体的IP地址
    client_addr.sin_port = htons(12346);  //端口
    bind(serverfd, (sockaddr*)&client_addr, len ) ;

    int nRecvBufLen = 0 ;
    connect( serverfd, (sockaddr*)&server_addr, len ) ;
    setsockopt( serverfd, SOL_SOCKET, SO_RCVBUF, ( const char* )&nRecvBufLen, sizeof( int ) );
    setsockopt( serverfd, SOL_SOCKET, SO_SNDBUF, ( const char* )&nRecvBufLen, sizeof( int ) );
    while ( 1 ) {
        //cout << "请输入:" ;
        //cin >> sndbuf ;
        strcpy( sndbuf, "12312312312" ) ;
        int wbyte = strlen(sndbuf) ;
        memset( rcvbuf, 0, sizeof(rcvbuf) ) ;
        {
            boost::timer::auto_cpu_timer t ;
            //write( serverfd, sndbuf, wbyte ) ;
            //read( serverfd, rcvbuf, sizeof(rcvbuf)) ;
            //sendto( serverfd, sndbuf, wbyte, 0, (sockaddr*)&server_addr, len ) ;
            //sendto( serverfd, sndbuf, wbyte, 0, nullptr, 0 ) ;
	        //recvfrom( serverfd, rcvbuf, wbyte, 0, (sockaddr*)&server_addr, &len ) ;
            //recv_loop( serverfd, rcvbuf, wbyte ) ;
            recv_loop( serverfd, rcvbuf, sendto( serverfd, sndbuf, wbyte, 0, nullptr, 0 ), nullptr, 0 ) ;
        }
        printf( "rcv %s\n", rcvbuf ) ;
    }
}
