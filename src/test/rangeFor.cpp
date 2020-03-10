#include <iostream>
using namespace std;

class Arr
{
public:
    Arr()
        :_arr{0},
        _iter(this)
    {}

    class iterator
    {
    public:
        iterator(Arr *parent, int pos = 0)
            :_pos(pos),
             _this(parent)
        {}
        iterator operator++()
        {
            ++_pos;
            return *this;
        }
        int &operator*()
        {
            return _this->_arr[_pos];
        }
        bool operator!=(const iterator &th)const
        {
            return _this != th._this || _pos != th._pos;
        }
    private:
        int _pos;
        Arr *_this;
    };
    iterator begin()
    {
        return iterator(this);
    }
    iterator end()
    {
        return iterator(this, sizeof(_arr)/sizeof(*_arr));
    }

private:
    int _arr[10];
    iterator _iter;
};

int main()
{
    Arr arr;
    for (auto &v : arr)
    {
        v = 10;
    }
    for (auto v : arr)
    {
        cout << v << endl;
    }
}
