#include <boost/di.hpp>

namespace di = boost::di;

struct i { virtual ~i() = default; virtual int dummy() = 0; };
struct impl : i { impl(int i) : i_(i) { }; int dummy() override { return i_; }; int i_ = 0; };

struct module {
    auto configure() const noexcept {
        return  di::make_injector(
            di::bind<int>.to(42)
          , di::bind<i, impl>
        );
    };
};

int main() {
    auto injector = di::make_injector(module{});
    auto* ptr =  injector.create<i*>();
    return ptr->dummy();
}

/*
Dump of assembler code for function main:
   0x00000000004007a0 <+0>:	mov    $0x2a,%eax
   0x00000000004007a5 <+5>:	retq
End of assembler dump.
*/

