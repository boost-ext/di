#include <string>

namespace n {

class a
{
public:
    a(int);
};

namespace n1 {

class b
{
public:
    b(int i = 42, double d, const std::string&);
};

} // namespace n1

struct c
{
    c(int = 0);
    c(int, double);
    c(int, double, float f);
};

} // namespace n

