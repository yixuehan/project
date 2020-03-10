#include <iostream>
#include <thread>
using namespace std;

class A
{
public:
    A()
        :_p(new int(0))
    {
        cout << "构造" << endl;
    }
    A(A&&a)
    {
        cout << "构造&&" << endl;
        _p = a._p;
        a._p = nullptr;
    }
    ~A()
    {
        delete _p;
    }
//private:
    int *_p;
};

void funcR(A&& a)
{
    cout << a._p << endl;
}

int main()
{
    A a;
    cout << a._p << endl;
    auto t1 = thread(funcR, std::move(a));
    t1.join();
}
