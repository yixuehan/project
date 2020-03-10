#include <iostream>
using namespace std;

#include "myArr.hpp"
#include "tool/debug.hpp"

template <typename T1, typename T2>
class MyClass
{
public:
    MyClass(T1 t1, T2 t2)
        :_t1(t1),
         _t2(t2)
    {}
    void print()
    {
        Debug() << _t1 << "," << _t2;
    }
private:
    T1 _t1;
    T2 _t2;
};

int main()
{
    MyClass<int, double> m(10, 22.2);
    m.print();
    MyClass<double, string> m2(20.8, "hello Template");
    m2.print();
    MyArray<int, 10> a1;
    for (size_t i = 0;i < a1.size(); ++i){
        a1[i] = i;
        Debug() << a1[i];
    }
}
