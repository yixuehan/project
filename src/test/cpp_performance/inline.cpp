#include <boost/timer/timer.hpp>
int calc (int a, int b)
{
    return a + b;
}
inline int calc2 (int a, int b)
{
    return a + b;
}

const int length = 10000;

int main()
{
    int x[length];
    int y[length];
    int z[length];

    {
        boost::timer::auto_cpu_timer t;
        for (int i = 0; i < length; ++i) {
            for (int j = 0; j < length; ++j) {
                for (int k = 0; k < length; ++k) {
                    z[i] = calc(y[j], x[k]);
                    int m = z[i];
                    z[i] = m;
                }
            }
        }
    }
    {
        boost::timer::auto_cpu_timer t;
        for (int i = 0; i < length; ++i) {
            for (int j = 0; j < length; ++j) {
                for (int k = 0; k < length; ++k) {
                    z[i] = calc2(y[j], x[k]);
                    int m = z[i];
                    z[i] = m;
                }
            }
        }
    }
}
