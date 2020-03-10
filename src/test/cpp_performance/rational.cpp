#include <iostream>
#include <boost/timer/timer.hpp>
#include <boost/pool/pool.hpp>

class Rational
{
public:
    Rational(int a = 0, int b = 1) :
        _n(a),
        _d(b)
    {}
private:
    int _n;
    int _d;
};

class NextOnFreeList
{
public:
    NextOnFreeList *next ;
};

class Rational2
{
public:
    Rational2(int a = 0, int b = 1) :
        _n(a),
        _d(b)
    {}
    inline void *operator new(size_t size);
    inline void operator delete(void *doomed, size_t size);

    static void newMemPool() {expandTheFreeList();}
    static void deleteMemPool();
private:
    static NextOnFreeList *freeList;

    static void expandTheFreeList();
    enum {EXPANSION_SIZE = 1000};

    int _n;
    int _d;
};

inline void *Rational2::operator new(size_t size)
{
    if (nullptr == freeList) {
        expandTheFreeList();
    }
    auto head = freeList;
    freeList = head->next;
    return head;
}

inline void Rational2::operator delete(void *doomed, size_t size)
{
    auto *head = static_cast<NextOnFreeList *>(doomed);
    head->next = freeList;
    freeList = head;
}

void Rational2::expandTheFreeList()
{
    size_t size = (sizeof(Rational) > sizeof(NextOnFreeList*)) ?
        sizeof(Rational) : sizeof(NextOnFreeList*);
    NextOnFreeList *runner = (NextOnFreeList*)(new char[size]);
    freeList = runner;
    for (int i = 0; i < EXPANSION_SIZE; ++i) {
        runner->next = (NextOnFreeList*)(new char[size]);
        runner = runner->next;
    }
    runner->next = 0;
}

void Rational2::deleteMemPool()
{
    NextOnFreeList *next;
    for (next = freeList; next != nullptr; next = freeList) {
        freeList = freeList->next;
        delete[] next;
    }
}

NextOnFreeList *Rational2::freeList = 0;

int main()
{
    {
        Rational *array[1000];
        boost::timer::auto_cpu_timer t;
        for (int j = 0; j < 500; ++j) {
            for (int i = 0; i < 1000; ++i) {
                array[i] = new Rational(i);
            }
            for (int i = 0; i < 1000; ++i) {
                delete array[i];
            }
        }
    }
    {
        Rational2 *array[1000];
        Rational2::newMemPool();
        {
            boost::timer::auto_cpu_timer t;
            for (int j = 0; j < 500; ++j) {
                for (int i = 0; i < 1000; ++i) {
                    array[i] = new Rational2(i);
                }
                for (int i = 0; i < 1000; ++i) {
                    delete array[i];
                }
            }
        }
        Rational2::deleteMemPool();
    }
    {
        boost::pool<> p(sizeof(Rational));
        Rational *array[1000];
        {
            boost::timer::auto_cpu_timer t;
            for (int j = 0; j < 500; ++j) {
                for (int i = 0; i < 1000; ++i) {
                    array[i] = (Rational*)p.malloc();
                }
                //for (int i = 0; i < 1000; ++i) {
                //    p.free(array[i]);
                //}
            }
        }
    }
}
