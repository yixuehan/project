#include "switch_string.hpp"

#include <cstdio>

void _print128( __uint128_t u )
{
    if ( u / 2 )
        _print128( u / 2 ) ;
    putchar( (u % 2) + '0' ) ;
}

void print128( __uint128_t u )
{
    _print128( u ) ;
    putchar('\n') ;
}

