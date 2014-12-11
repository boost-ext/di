//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

//[module1_hpp_cpp_11
//````module1.hpp```
#include <boost/di.hpp>

namespace di = boost::di;

class implementation1;
class implementation2;
class implementation3;
class implementation4;
class implementation5;
class implementation6;
class implementation7;
class implementation8;
class implementation9;
class implementation10;
class implementation11;
class implementation12;
class implementation13;
class implementation14;
class implementation15;
class implementation16;
class implementation17;
class implementation18;
class implementation19;
class implementation20;

class module1 {
    using injector = di::injector<
        di::deduce<implementation1>
      , di::deduce<implementation2>
      , di::deduce<implementation3>
      , di::deduce<implementation4>
      , di::deduce<implementation5>
      , di::deduce<implementation6>
      , di::deduce<implementation7>
      , di::deduce<implementation8>
      , di::deduce<implementation9>
      , di::deduce<implementation10>
      , di::deduce<implementation11>
      , di::deduce<implementation12>
      , di::deduce<implementation13>
      , di::deduce<implementation14>
      , di::deduce<implementation15>
      , di::deduce<implementation16>
      , di::deduce<implementation17>
      , di::deduce<implementation18>
      , di::deduce<implementation19>
      , di::deduce<implementation20>
      , decltype(di::bind<bool>::to(bool()))
    >;

public:
    explicit module1(bool flag)
        : flag_(flag)
    { }

    injector configure() const;

private:
    bool flag_ = false;
};

//]

