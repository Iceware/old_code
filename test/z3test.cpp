#include "z3++.h"
#include <iostream>
#include <string>
using namespace std;
void demorgan()
{
    cout << "de-morgan example\n";
    z3::context c;
    z3::expr x = c.bool_const("x");
    z3::expr y = c.bool_const("y");
    z3::expr conjecture = (!(x && y) == (!x || !y));

    z3::solver s(c);
    s.add(!conjecture);
    cout << s << "\n";
    cout << s.to_smt2() << "\n";
    switch (s.check())
    {
    case z3::unsat:
        cout << "valid\n";
        break;
    case z3::sat:
        cout << "not valid\n";
        break;
    case z3::unknown:
        cout << "unknown\n";
        break;
    }
}

int main(int argc, char **argv)
{
    try
    {
        demorgan();
    }
    catch (z3::exception &ex)
    {
        cout << "unexpected error " << ex << '\n';
    }
}