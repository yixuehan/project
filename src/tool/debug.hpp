#pragma once

#include <cstdio>
#include <sstream>

namespace trade{
namespace tool{

class Debug_t
{
public:
    Debug_t(FILE *fp = stdout)
        :_fp(fp)
    {}

    template <typename T>
    Debug_t &operator<<( const T& t)
    {
        if (_ostr.str().size()) {
            _ostr << ' ';
        }
        _ostr << t ;
        return *this;
    }

    ~Debug_t()
    {
        _ostr << "\n";
        fwrite(_ostr.str().c_str(), 1, _ostr.str().size(), _fp);
    }
private:
    std::ostringstream _ostr;
    FILE *_fp;
};

#ifndef Debug
#define Debug trade::tool::Debug_t
#endif

} // namespace tool
} // namespace trade

