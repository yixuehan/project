#include <unistd.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <sys/epoll.h>

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
#define MAXEVENTS 10
boost::container::small_vector<int, 1024> connfds ;

int main()
{
    int listenfd = bind_listen( "0.0.0.0", 12345 ) ;
    int flag = 1 ;
    struct timeval ts ;
    ts.tv_sec = 0 ;
    ts.tv_usec = 0 ;
    char on = 1 ;
    int efd = epoll_create(10) ;
    struct epoll_event ev, events[MAXEVENTS] ;
    ev.events = EPOLLIN ;
    ev.data.fd = listenfd ;
    if (epoll_ctl(efd, EPOLL_CTL_ADD, listenfd, &ev) == -1) {
        perror("epoll_ctl: listen_sock");
        exit(EXIT_FAILURE);
    }
    while( 1 ) {
        int num = epoll_wait( efd, events, MAXEVENTS, 0 ) ;
        for ( int i = 0;i < num; ++i ) {
            if ( events[i].data.fd == listenfd ) {
                printf( "连接\n" ) ;
                int fd = accept( listenfd, NULL, NULL ) ;
                if (-1 == setsockopt( fd, IPPROTO_TCP, TCP_NODELAY, &flag, 4 ) ) {
                    perror( "setsockopt" ) ;
                    return -1 ;
                }
                int nRecvBufLen = 0 ;
                setsockopt( fd, SOL_SOCKET, SO_RCVBUF, &nRecvBufLen, sizeof( int ) );
                setsockopt( fd, SOL_SOCKET, SO_SNDBUF, &nRecvBufLen, sizeof( int ) );
                setsockopt( fd, IPPROTO_TCP, TCP_QUICKACK, &on, sizeof(on) );
                ev.data.fd = fd ;
                ev.events = EPOLLIN ;
                epoll_ctl( efd, EPOLL_CTL_ADD, fd, &ev ) ;
            }
            else if ( events[i].events & EPOLLIN ) {
                //printf( "读\n" ) ;
                int fd = events[i].data.fd ;
                int rbyte = recv( fd, buff, sizeof(buff), 0 ) ;
                if ( 0 >= rbyte ) {
                    printf( "rbyte:%d\n", rbyte) ;
                    close(fd) ;
                    ev.data.fd = fd ;
                    epoll_ctl(efd, EPOLL_CTL_DEL, fd, nullptr) ;
                    printf( "客户端退出\n" ) ;
                    continue ;
                }
                send( fd, buff, rbyte, 0 ) ;
                setsockopt(fd, IPPROTO_TCP, TCP_QUICKACK, &on, sizeof(on));
            }
            else if ( events[i].events & EPOLLERR ) {
                printf( "错误\n" ) ;
                int fd = events[i].data.fd ;
                close( fd ) ;
                ev.data.fd = fd ;
                epoll_ctl(efd, EPOLL_CTL_DEL, fd, nullptr) ;
                printf( "客户端退出ERR\n" ) ;
            }
        }
        //printf( "continue num %d\n", num ) ;
        //if (0 == num){
        //    printf( "continue num %d\n", num ) ;
        //    continue ;
        //}
    }
}
