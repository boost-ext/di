//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

//[app_cpp_cpp_11
//````app.cpp```
//<-
#include <cassert>
//->
#include "app.hpp"
#include "data.hpp"

app1::app1(std::unique_ptr<interface1> i1
         , std::unique_ptr<interface2> i2
         , std::unique_ptr<interface3> i3
         , std::unique_ptr<interface4> i4
         , std::unique_ptr<interface5> i5
         , std::unique_ptr<interface6> i6
         , std::unique_ptr<interface7> i7
         , std::unique_ptr<interface8> i8
         , std::unique_ptr<interface9> i9
         , std::unique_ptr<interface10> i10)
{
    assert(dynamic_cast<implementation1*>(i1.get()));
    assert(dynamic_cast<implementation2*>(i2.get()));
    assert(dynamic_cast<implementation3*>(i3.get()));
    assert(dynamic_cast<implementation4*>(i4.get()));
    assert(dynamic_cast<implementation5*>(i5.get()));
    assert(dynamic_cast<implementation6*>(i6.get()));
    assert(dynamic_cast<implementation7*>(i7.get()));
    assert(dynamic_cast<implementation8*>(i8.get()));
    assert(dynamic_cast<implementation9*>(i9.get()));
    assert(dynamic_cast<implementation10*>(i10.get()));
}

app2::app2(std::unique_ptr<interface11> i11
         , std::unique_ptr<interface12> i12
         , std::unique_ptr<interface13> i13
         , std::unique_ptr<interface14> i14
         , std::unique_ptr<interface15> i15
         , std::unique_ptr<interface16> i16
         , std::unique_ptr<interface17> i17
         , std::unique_ptr<interface18> i18
         , std::unique_ptr<interface19> i19
         , std::unique_ptr<interface20> i20)
{
    assert(dynamic_cast<implementation11*>(i11.get()));
    assert(dynamic_cast<implementation12*>(i12.get()));
    assert(dynamic_cast<implementation13*>(i13.get()));
    assert(dynamic_cast<implementation14*>(i14.get()));
    assert(dynamic_cast<implementation15*>(i15.get()));
    assert(dynamic_cast<implementation16*>(i16.get()));
    assert(dynamic_cast<implementation17*>(i17.get()));
    assert(dynamic_cast<implementation18*>(i18.get()));
    assert(dynamic_cast<implementation19*>(i19.get()));
    assert(dynamic_cast<implementation20*>(i20.get()));
}

app3::app3(std::unique_ptr<interface21> i21
         , std::unique_ptr<interface22> i22
         , std::unique_ptr<interface23> i23
         , std::unique_ptr<interface24> i24
         , std::unique_ptr<interface25> i25
         , std::unique_ptr<interface26> i26
         , std::unique_ptr<interface27> i27
         , std::unique_ptr<interface28> i28
         , std::unique_ptr<interface29> i29
         , std::unique_ptr<interface30> i30)
{
    assert(dynamic_cast<implementation21*>(i21.get()));
    assert(dynamic_cast<implementation22*>(i22.get()));
    assert(dynamic_cast<implementation23*>(i23.get()));
    assert(dynamic_cast<implementation24*>(i24.get()));
    assert(dynamic_cast<implementation25*>(i25.get()));
    assert(dynamic_cast<implementation26*>(i26.get()));
    assert(dynamic_cast<implementation27*>(i27.get()));
    assert(dynamic_cast<implementation28*>(i28.get()));
    assert(dynamic_cast<implementation29*>(i29.get()));
    assert(dynamic_cast<implementation30*>(i30.get()));
}

app4::app4(std::unique_ptr<interface31> i31
         , std::unique_ptr<interface32> i32
         , std::unique_ptr<interface33> i33
         , std::unique_ptr<interface34> i34
         , std::unique_ptr<interface35> i35
         , std::unique_ptr<interface36> i36
         , std::unique_ptr<interface37> i37
         , std::unique_ptr<interface38> i38
         , std::unique_ptr<interface39> i39
         , std::unique_ptr<interface40> i40)
{
    assert(dynamic_cast<implementation31*>(i31.get()));
    assert(dynamic_cast<implementation32*>(i32.get()));
    assert(dynamic_cast<implementation33*>(i33.get()));
    assert(dynamic_cast<implementation34*>(i34.get()));
    assert(dynamic_cast<implementation35*>(i35.get()));
    assert(dynamic_cast<implementation36*>(i36.get()));
    assert(dynamic_cast<implementation37*>(i37.get()));
    assert(dynamic_cast<implementation38*>(i38.get()));
    assert(dynamic_cast<implementation39*>(i39.get()));
    assert(dynamic_cast<implementation40*>(i40.get()));
}

app::app(std::shared_ptr<app1> a1
       , std::shared_ptr<app2> a2
       , std::shared_ptr<app3> a3
       , std::shared_ptr<app4> a4
       , bool flag
       , int value)
    : flag_(flag)
    , value_(value)
{
    assert(a1.get());
    assert(a2.get());
    assert(a3.get());
    assert(a4.get());
}

int app::run() const {
    return !flag_ ? value_ : 0;
}

//]

