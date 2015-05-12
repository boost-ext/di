//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/di.hpp>

namespace di = boost::di;

int main() {
    auto injector = di::make_injector();
    return injector.create<int>();

    /*<< asm gdb -batch -ex 'file ./a.out' -ex 'disassemble main' [pre
        Dump of assembler code for function main:
        0x0000000000400760 <+0>:    xor    %eax,%eax
        0x0000000000400762 <+2>:    retq
        End of assembler dump.
    ]>>*/
}

