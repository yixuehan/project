#include <unistd.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cerrno>

char on = 1 ;

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

char buf[4096] ;

inline int recv_loop( int fd, void *buf, size_t len )
{
    int rc = -1 ;
    int on = 1 ;
    while ( 0 >= rc ) {
        rc =::recv( fd, buf, len, MSG_DONTWAIT ) ;
        if ( -1 == rc ) {
            setsockopt(fd, IPPROTO_TCP, TCP_QUICKACK, &on, sizeof(on));
            continue ;
        }
        //if ( (-1 == rc && (EAGAIN == errno || EINTR == errno || EWOULDBLOCK == errno)) ) continue ;
        if ( !rc ) {
            printf( "rc %d errno %d\n", rc, errno ) ;
            return -1 ;
        }
        setsockopt(fd, IPPROTO_TCP, TCP_QUICKACK, &on, sizeof(on));
        return rc ;
    }
    return rc ;
}

int main()
{
    int listenfd = bind_listen( "127.0.0.1", 12345 ) ;
    int clientfd = accept( listenfd, nullptr, nullptr ) ;
    setsockopt(clientfd, IPPROTO_TCP, TCP_QUICKACK, &on, sizeof(on));
    int flag = 1 ;
    if (-1 == setsockopt( clientfd, IPPROTO_TCP, TCP_NODELAY, &flag, 4 ) ) {
        perror( "setsockopt" ) ;
        return -1 ;
    }
    //int on = 1;
    //setsockopt(clientfd, SOL_TCP, TCP_CORK, &on, sizeof(on));
    int nRecvBufLen = 0; //设置为0
    setsockopt( clientfd, SOL_SOCKET, SO_RCVBUF, ( const char* )&nRecvBufLen, sizeof( int ) );
    setsockopt( clientfd, SOL_SOCKET, SO_SNDBUF, ( const char* )&nRecvBufLen, sizeof( int ) );
    //int rbyte ;
    while( 1 ) {
        //recv_loop( clientfd, buf, sizeof(buf) ) ;
        send( clientfd, buf, recv_loop( clientfd, buf, sizeof(buf) ), 0 ) ;
    }
}
