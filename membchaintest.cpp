
#include "nest_mem.h"
#include <vector>
#include <iostream>

struct A
{
    int value = 100;
};

struct B
{
    A a;
};

struct C
{
    B b;
};

int main(void)
{
    using test = intrusive::member_chain<
        intrusive::member_pointer<C, B, &C::b>,
        intrusive::member_pointer<B, A, &B::a>,
        intrusive::member_pointer<A, int, &A::value>
    >;

    std::vector<C> cvals;
    for (int i = 0; i < 10; ++i)
    {
        cvals.push_back(C{B{A{i}}});
    }

    for (C& c : cvals)
    {
        std::cout << *test::resolve(&c) << std::endl;
    }

}

