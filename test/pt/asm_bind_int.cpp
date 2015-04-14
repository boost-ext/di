//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/di.hpp>

namespace di = boost::di;

int main() {
    auto injector = di::make_injector(
        di::bind<int>.to(42)
    );

    return injector.create<int>();

    /*<< asm gdb -batch -ex 'file ./a.out' -ex 'disassemble main' [pre
        Dump of assembler code for function main:
           0x0000000000400780 <+0>:    mov    $0x2a,%eax
           0x0000000000400785 <+5>:    retq
        End of assembler dump.
    ]>>*/
}

