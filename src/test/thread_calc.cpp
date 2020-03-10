#include<stdio.h>
#include<time.h>
#include <thread>
#include <future>
#include <vector>
using namespace std;

#include <boost/timer/timer.hpp>

long long mycalc(long long s, long long e) {
    long long c = 0;
    while ( s < e ) {
        c += s;
        ++s;
    }
    return c;
}

int  main(int argc, char **argv)
{
    long long i;
    long long count = 9000000000ll;

    boost::timer::auto_cpu_timer t;
    long long sum = 0;
    int CPU_NUMS = std::thread::hardware_concurrency();
    printf("CPUNUMS %d\n", CPU_NUMS);
    vector<std::future<long long>> ress ;
    long long step = count / CPU_NUMS;
    for (i = 0 ; i < CPU_NUMS; i++){
        ress.push_back(std::async(std::launch::async, mycalc, i * step, (i+1)*step));
    }
    for (auto &res : ress) {
        sum += res.get();
    }

    printf("count:%lld\n", sum);

    return 0;
}
