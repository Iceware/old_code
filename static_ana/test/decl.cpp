enum class Cpp11Enum
{
    RED = 10,
    BLUE = 20
};

struct Wowza
{
    virtual ~Wowza() = default;
    virtual void foo(int a, int b) = 0;
};

struct Badabang : public Wowza
{
    void foo(int, int) override;
    bool operator==(const Badabang &o) const;
};

template <typename T>
void bar(T &&t);