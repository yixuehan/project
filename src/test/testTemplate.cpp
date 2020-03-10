#include <iostream>
using namespace std ;

void myswap(int &x, int &y)
{
    cout << "func swap\n";
}

template <typename T>
void myswap(T &x, T &y)
{
    cout << "template swap\n";
}

int main()
{
    int x, y;
    myswap<>(x, y);
}
