#include <iostream>
#include <unistd.h>
#include <fcntl.h>

int main()
{
    srand(getpid()) ;
    int fd = open("a.txt", O_CREAT | O_WRONLY, 0200) ;
    if (-1 == fd) {
        perror("open") ;
    }

    //close(fd) ;
    printf("%d\n", O_ACCMODE) ;

    int flag = fcntl(fd, F_GETFL) ;
    flag |= O_FSYNC ;
    fcntl(fd, F_SETFL, flag) ;
    for (int i = 0; i < 10; ++i) {
        char ch = '0' + i ;
        printf( "write %ld\n", write(fd, &ch, 1) ) ;
        //fsync(fd) ;
        printf("%c", ch) ;
        fflush(stdout) ;
        //sleep((rand()%4)+1) ;
    }


    close(fd) ;
}
