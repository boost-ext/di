//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/injector.hpp"
#include "boost/di/named.hpp"

#include <boost/test/unit_test.hpp>

namespace boost {
namespace di {

struct n1 { };
struct n2 { };
struct n3 { };
struct n4 { };
struct n5 { };
struct n6 { };
struct n7 { };
struct n8 { };
struct n9 { };
struct n10 { };
struct n11 { };
struct n12 { };
struct n13 { };
struct n14 { };
struct n15 { };
struct n16 { };
struct n17 { };
struct n18 { };
struct n19 { };
struct n20 { };
struct n21 { };
struct n22 { };
struct n23 { };
struct n24 { };
struct n25 { };
struct n26 { };
struct n27 { };
struct n28 { };
struct n29 { };
struct n30 { };

struct limit
{
    limit(
          named<int, n1> i1
        , named<int, n2> i2
        , named<int, n3> i3
        , named<int, n4> i4
        , named<int, n5> i5
        , named<int, n6> i6
        , named<int, n7> i7
        , named<int, n8> i8
        , named<int, n9> i9
        , named<int, n10> i10
        , named<int, n11> i11
        , named<int, n12> i12
        , named<int, n13> i13
        , named<int, n14> i14
        , named<int, n15> i15
        , named<int, n16> i16
        , named<int, n17> i17
        , named<int, n18> i18
        , named<int, n19> i19
        , named<int, n20> i20
        , named<int, n21> i21
        , named<int, n22> i22
        , named<int, n23> i23
        , named<int, n24> i24
        , named<int, n25> i25
        , named<int, n26> i26
        , named<int, n27> i27
        , named<int, n28> i28
        , named<int, n29> i29
        , named<int, n30> i30
    )
        : i1(i1)
        , i2(i2)
        , i3(i3)
        , i4(i4)
        , i5(i5)
        , i6(i6)
        , i7(i7)
        , i8(i8)
        , i9(i9)
        , i10(i10)
        , i11(i11)
        , i12(i12)
        , i13(i13)
        , i14(i14)
        , i15(i15)
        , i16(i16)
        , i17(i17)
        , i18(i18)
        , i19(i19)
        , i20(i20)
        , i21(i21)
        , i22(i22)
        , i23(i23)
        , i24(i24)
        , i25(i25)
        , i26(i26)
        , i27(i27)
        , i28(i28)
        , i29(i29)
        , i30(i30)
    { }

    int i1;
    int i2;
    int i3;
    int i4;
    int i5;
    int i6;
    int i7;
    int i8;
    int i9;
    int i10;
    int i11;
    int i12;
    int i13;
    int i14;
    int i15;
    int i16;
    int i17;
    int i18;
    int i19;
    int i20;
    int i21;
    int i22;
    int i23;
    int i24;
    int i25;
    int i26;
    int i27;
    int i28;
    int i29;
    int i30;
};

BOOST_AUTO_TEST_CASE(di_limit_size) {
    limit limit_ = injector<
        bind_int<1>::in_name<n1>
      , bind_int<2>::in_name<n2>
      , bind_int<3>::in_name<n3>
      , bind_int<4>::in_name<n4>
      , bind_int<5>::in_name<n5>
      , bind_int<6>::in_name<n6>
      , bind_int<7>::in_name<n7>
      , bind_int<8>::in_name<n8>
      , bind_int<9>::in_name<n9>
      , bind_int<10>::in_name<n10>
      , bind_int<11>::in_name<n11>
      , bind_int<12>::in_name<n12>
      , bind_int<13>::in_name<n13>
      , bind_int<14>::in_name<n14>
      , bind_int<15>::in_name<n15>
      , bind_int<16>::in_name<n16>
      , bind_int<17>::in_name<n17>
      , bind_int<18>::in_name<n18>
      , bind_int<19>::in_name<n19>
      , bind_int<20>::in_name<n20>
      , bind_int<21>::in_name<n21>
      , bind_int<22>::in_name<n22>
      , bind_int<23>::in_name<n23>
      , bind_int<24>::in_name<n24>
      , bind_int<25>::in_name<n25>
      , bind_int<26>::in_name<n26>
      , bind_int<27>::in_name<n27>
      , bind_int<28>::in_name<n28>
      , bind_int<29>::in_name<n29>
      , bind_int<30>::in_name<n30>
    >().create<limit>();

    BOOST_CHECK_EQUAL(1, limit_.i1);
    BOOST_CHECK_EQUAL(2, limit_.i2);
    BOOST_CHECK_EQUAL(3, limit_.i3);
    BOOST_CHECK_EQUAL(4, limit_.i4);
    BOOST_CHECK_EQUAL(5, limit_.i5);
    BOOST_CHECK_EQUAL(6, limit_.i6);
    BOOST_CHECK_EQUAL(7, limit_.i7);
    BOOST_CHECK_EQUAL(8, limit_.i8);
    BOOST_CHECK_EQUAL(9, limit_.i9);
    BOOST_CHECK_EQUAL(10, limit_.i10);
    BOOST_CHECK_EQUAL(11, limit_.i11);
    BOOST_CHECK_EQUAL(12, limit_.i12);
    BOOST_CHECK_EQUAL(13, limit_.i13);
    BOOST_CHECK_EQUAL(14, limit_.i14);
    BOOST_CHECK_EQUAL(15, limit_.i15);
    BOOST_CHECK_EQUAL(16, limit_.i16);
    BOOST_CHECK_EQUAL(17, limit_.i17);
    BOOST_CHECK_EQUAL(18, limit_.i18);
    BOOST_CHECK_EQUAL(19, limit_.i19);
    BOOST_CHECK_EQUAL(20, limit_.i20);
    BOOST_CHECK_EQUAL(21, limit_.i21);
    BOOST_CHECK_EQUAL(22, limit_.i22);
    BOOST_CHECK_EQUAL(23, limit_.i23);
    BOOST_CHECK_EQUAL(24, limit_.i24);
    BOOST_CHECK_EQUAL(25, limit_.i25);
    BOOST_CHECK_EQUAL(26, limit_.i26);
    BOOST_CHECK_EQUAL(27, limit_.i27);
    BOOST_CHECK_EQUAL(28, limit_.i28);
    BOOST_CHECK_EQUAL(29, limit_.i29);
    BOOST_CHECK_EQUAL(30, limit_.i30);
}

} // namespace di
} // namespace boost

