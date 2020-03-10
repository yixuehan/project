#include<stdio.h>
#include<time.h>

int  main(int argc, char **argv)
{
    int second1,second2;
    long long i;
    long long count;

    second1 = time((time_t*)NULL);
    count = 0;
    for (i = 0 ; i < 9000000000ll; i++){
            count += i;
        }
    second2 = time((time_t*)NULL);
    printf("cost:%d,count:%lld\n", second2 - second1, count);

    return 0;
}
