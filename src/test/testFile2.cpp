#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <cstdio>
using namespace std ;

int main()
{
    srand(getpid()) ;
    int fd = open("a.txt", O_CREAT | O_WRONLY, 0200) ;
    if (-1 == fd) {
        perror("open") ;
    }
    for (int i = 0; i < 10; ++i) {
        char ch = 'A' + i ;
        printf( "write %ld\n", write(fd, &ch, 1) ) ;
        printf("%c", ch) ;
        fflush(stdout) ;
        //fsync(fd) ;
        sleep((rand()%4)+1) ;
    }


    close(fd) ;
}
