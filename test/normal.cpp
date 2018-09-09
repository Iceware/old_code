#include <iostream>
#include "normal.hpp"

int foo1();
int foo() { return 1; }
int main(int argc, char **argv)
{
    A a;
    std::cout << a.var_ << std::endl;
    foo();
}

int foo1()
{
    return 0;
}