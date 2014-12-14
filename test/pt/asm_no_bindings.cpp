#include <boost/di.hpp>

namespace di = boost::di;

int main() {
    auto injector = di::make_injector();
    return injector.create<int>();
}

/*
#gdb -batch -ex 'file ./a.out' -ex 'disassemble main'

Dump of assembler code for function main:
   0x0000000000400760 <+0>:	xor    %eax,%eax
   0x0000000000400762 <+2>:	retq
End of assembler dump.
*/

