//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

//[module2_hpp_cpp_11
//````module2.hpp```
#include <boost/di.hpp>

namespace di = boost::di;

class implementation20;
class implementation21;
class implementation22;
class implementation23;
class implementation24;
class implementation25;
class implementation26;
class implementation27;
class implementation28;
class implementation29;
class implementation30;
class implementation31;
class implementation32;
class implementation33;
class implementation34;
class implementation35;
class implementation36;
class implementation37;
class implementation38;
class implementation39;
class implementation40;

class module2 {
    using injector = di::injector<
        di::deduce<implementation21>
      , di::deduce<implementation22>
      , di::deduce<implementation23>
      , di::deduce<implementation24>
      , di::deduce<implementation25>
      , di::deduce<implementation26>
      , di::deduce<implementation27>
      , di::deduce<implementation28>
      , di::deduce<implementation29>
      , di::deduce<implementation30>
      , di::deduce<implementation31>
      , di::deduce<implementation32>
      , di::deduce<implementation33>
      , di::deduce<implementation34>
      , di::deduce<implementation35>
      , di::deduce<implementation36>
      , di::deduce<implementation37>
      , di::deduce<implementation38>
      , di::deduce<implementation39>
      , di::deduce<implementation40>
      , decltype(di::bind<int>::to(int()))
    >;

public:
    explicit module2(int value)
        : value_(value)
    { }

    injector configure() const;

private:
    int value_ = 0;
};

//]

