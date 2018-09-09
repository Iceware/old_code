#include <iostream>
using namespace std;
int main(int argc, char **argv)
{
    int i = 1;
    int b = 6 % 2;
    for (int i = 0; i < 1; ++i)
    {
        b += 2;
        b %= 2;
    }

    cout << i / b << endl;
}
