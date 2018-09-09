#include "pure.h"
#define max(a, b) a
namespace stt
{
int a = 0;

int foo(int a)
{
    return a;
}
} // namespace stt

void foo(int a);
int main()
{
    A d;
    int a = 0;
    int b = 0;
    // {
    //     int a=0;
    // }
    foo(a);
    int c = max(a, b);
}

void foo(int a)
{
    a = 1;
}
