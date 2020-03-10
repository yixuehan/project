#include <iostream>

using namespace std;

constexpr size_t operator""_KB(unsigned long long n)
{
    return n * 1024;
}

int main()
{
    cout << 1_KB << endl;
}
