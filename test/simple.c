struct Chain
{
    int val;
};

int foo(){return 1;}

int main()
{
    struct Chain all[10];
    for (int i = 0; i < 10; ++i)
    {
        all[i].val = 100;
    }
    return 0;
}
