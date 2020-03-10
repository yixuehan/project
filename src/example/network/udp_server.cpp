#include <unistd.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cerrno>

int bind( const char *ip, unsigned short port )
{
    //创建套接字
    int serv_sock = socket(AF_INET, SOCK_DGRAM, 0);
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
    return serv_sock ;
}

char buf[4096] ;

inline int recv_loop( int fd, void *buf, size_t len, sockaddr* addr, socklen_t *pLen  )
{
    int rc = -1 ;
    while ( 0 >= rc ) {
        rc =::recvfrom( fd, buf, len, MSG_DONTWAIT, addr, pLen) ;
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

int main()
{
    int bindfd = bind( "127.0.0.1", 12345 ) ;
    sockaddr_in client ;
    socklen_t len = sizeof(client) ;
    int nRecvBufLen = 0 ;
    setsockopt( bindfd, SOL_SOCKET, SO_RCVBUF, ( const char* )&nRecvBufLen, sizeof( int ) );
    setsockopt( bindfd, SOL_SOCKET, SO_SNDBUF, ( const char* )&nRecvBufLen, sizeof( int ) );
    int rbyte = recv_loop( bindfd, buf, sizeof(buf), (sockaddr*)&client, &len ) ;
    connect( bindfd, (sockaddr*)&client, len ) ;
    while( 1 ) {
        //write( bindfd, buf, rbyte ) ;
        //rbyte = read( bindfd, buf, sizeof(buf)) ;
        //int rbyte = recvfrom( bindfd, buf, sizeof(buf), 0, (sockaddr*)&client, &len ) ;
        //printf( "recv %d\n", rbyte) ;
        //sendto( bindfd, buf, rbyte, 0, (sockaddr*)&client, len ) ;
        sendto( bindfd, buf, rbyte, 0, nullptr, 0 ) ;
        rbyte = recv_loop( bindfd, buf, sizeof(buf), nullptr, 0 ) ;
        //rbyte = recv_loop( bindfd, buf, sizeof(buf) ) ;
    }
}
