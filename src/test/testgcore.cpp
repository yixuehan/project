#include <signal.h>
#include <cassert>
#include <iostream>
#include <thread>
#include <mutex>
#include <unistd.h>

std::once_flag flag;

void write_core()
{
    call_once(flag, []{
        char cmd[32] = {0};
        sprintf(cmd, "gcore %u\n", getpid());
        system(cmd);
    });
}

class A
{
public:
    A()
    {
        sleep(1);
        std::cout << "A()" << std::endl;
    }
};

void createA()
{
    std::cout << "createA\n" ;
    static A a;
}

void sig( int x )
{
    printf("x[%d]\n", x);
    write_core();
}

int main()
{
    signal(SIGABRT, sig);
    assert(false && "test assert");
    std::thread threads[10];
    for ( size_t i = 0; i < sizeof(threads)/sizeof(*threads); ++i ) {
        threads[i] = std::thread(createA);
    }
    for ( size_t i = 0; i < sizeof(threads)/sizeof(*threads); ++i ) {
        threads[i].join();
    }
}
