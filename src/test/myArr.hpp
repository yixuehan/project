#pragma once
#include <iostream>

template <class T, size_t N>
class MyArray
{
public:
    MyArray()
        :_array{}
    {
    }
    ~MyArray()
    {
    }
    constexpr size_t size() const
    {
        return N;
    }
    T &operator[](int pos)
    {
        return _array[pos];
    }
private:
    T _array[N];
};
