#if 0
class TrivialCtor { };

class DefaultCtor
{
public:
    explicit DefaultCtor(int = 0) { }
};

class CustomCtor
{
public:
    explicit CustomCtor(int) { }
};

class Base { };
class Derived : Base { };

struct Basic
{
    QDEPS_CTOR(Basic,
        (pool),
    { })

    Pool< vector<int, std::string> > pool;
};

struct Complex
{
    QPOOL_CTOR(Complex,
        (pool)
        (i(42))
        (s("s"))
    ,
        {
            d = 42.0;
        }
    )

    Pool< vector<int, std::string> > pool;
    int i;
    std::string s;
    double d;
};

#endif

