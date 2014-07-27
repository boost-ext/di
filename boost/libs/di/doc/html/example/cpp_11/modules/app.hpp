//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef APP_HPP
#define APP_HPP

//[app_hpp_cpp_11
//````app.hpp```
//<-
#include <memory>
//->

class interface1;
class interface2;
class interface3;
class interface4;
class interface5;
class interface6;
class interface7;
class interface8;
class interface9;
class interface10;
class interface11;
class interface12;
class interface13;
class interface14;
class interface15;
class interface16;
class interface17;
class interface18;
class interface19;
class interface20;
class interface21;
class interface22;
class interface23;
class interface24;
class interface25;
class interface26;
class interface27;
class interface28;
class interface29;
class interface30;
class interface31;
class interface32;
class interface33;
class interface34;
class interface35;
class interface36;
class interface37;
class interface38;
class interface39;
class interface40;

class app1 {
public:
    app1(std::unique_ptr<interface1>
       , std::unique_ptr<interface2>
       , std::unique_ptr<interface3>
       , std::unique_ptr<interface4>
       , std::unique_ptr<interface5>
       , std::unique_ptr<interface6>
       , std::unique_ptr<interface7>
       , std::unique_ptr<interface8>
       , std::unique_ptr<interface9>
       , std::unique_ptr<interface10>);
};

class app2 {
public:
    app2(std::unique_ptr<interface11>
       , std::unique_ptr<interface12>
       , std::unique_ptr<interface13>
       , std::unique_ptr<interface14>
       , std::unique_ptr<interface15>
       , std::unique_ptr<interface16>
       , std::unique_ptr<interface17>
       , std::unique_ptr<interface18>
       , std::unique_ptr<interface19>
       , std::unique_ptr<interface20>);
};

class app3 {
public:
    app3(std::unique_ptr<interface21>
       , std::unique_ptr<interface22>
       , std::unique_ptr<interface23>
       , std::unique_ptr<interface24>
       , std::unique_ptr<interface25>
       , std::unique_ptr<interface26>
       , std::unique_ptr<interface27>
       , std::unique_ptr<interface28>
       , std::unique_ptr<interface29>
       , std::unique_ptr<interface30>);
};

class app4 {
public:
    app4(std::unique_ptr<interface31>
       , std::unique_ptr<interface32>
       , std::unique_ptr<interface33>
       , std::unique_ptr<interface34>
       , std::unique_ptr<interface35>
       , std::unique_ptr<interface36>
       , std::unique_ptr<interface37>
       , std::unique_ptr<interface38>
       , std::unique_ptr<interface39>
       , std::unique_ptr<interface40>);
};

class app {
public:
    app(std::shared_ptr<app1>
      , std::shared_ptr<app2>
      , std::shared_ptr<app3>
      , std::shared_ptr<app4>
      , bool flag
      , int value);

    int run() const;

private:
    bool flag_ = false;
    int value_ = false;
};

//<-
#endif
//->

//]

