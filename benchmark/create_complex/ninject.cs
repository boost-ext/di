//
// Copyright (c) 2012-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
using Ninject;

class X00 { public X00() { } }
class X01 { public X01(X00 p1) { } }
class X02 { public X02(X00 p1, X01 p2) { } }
class X03 { public X03(X00 p1, X01 p2, X02 p3) { } }
class X04 { public X04(X00 p1, X01 p2, X02 p3, X03 p4) { } }
class X05 { public X05(X00 p1, X01 p2, X02 p3, X03 p4, X04 p5) { } }
class X06 { public X06(X00 p1, X01 p2, X02 p3, X03 p4, X04 p5, X05 p6) { } }
class X07 { public X07(X00 p1, X01 p2, X02 p3, X03 p4, X04 p5, X05 p6, X06 p7) { } }
class X08 { public X08(X00 p1, X01 p2, X02 p3, X03 p4, X04 p5, X05 p6, X06 p7, X07 p8) { } }
class X09 { public X09(X00 p1, X01 p2, X02 p3, X03 p4, X04 p5, X05 p6, X06 p7, X07 p8, X08 p9) { } }
class X10 { public X10(X00 p1, X01 p2, X02 p3, X03 p4, X04 p5, X05 p6, X06 p7, X07 p8, X08 p9, X09 p10) { } }
class X11 { public X11(X01 p1, X02 p2, X03 p3, X04 p4, X05 p5, X06 p6, X07 p7, X08 p8, X09 p9, X10 p10) { } }
class X12 { public X12(X02 p1, X03 p2, X04 p3, X05 p4, X06 p5, X07 p6, X08 p7, X09 p8, X10 p9, X11 p10) { } }
class X13 { public X13(X03 p1, X04 p2, X05 p3, X06 p4, X07 p5, X08 p6, X09 p7, X10 p8, X11 p9, X12 p10) { } }
class X14 { public X14(X04 p1, X05 p2, X06 p3, X07 p4, X08 p5, X09 p6, X10 p7, X11 p8, X12 p9, X13 p10) { } }
class X15 { public X15(X05 p1, X06 p2, X07 p3, X08 p4, X09 p5, X10 p6, X11 p7, X12 p8, X13 p9, X14 p10) { } }
class X16 { public X16(X06 p1, X07 p2, X08 p3, X09 p4, X10 p5, X11 p6, X12 p7, X13 p8, X14 p9, X15 p10) { } }
class X17 { public X17(X07 p1, X08 p2, X09 p3, X10 p4, X11 p5, X12 p6, X13 p7, X14 p8, X15 p9, X16 p10) { } }
class X18 { public X18(X08 p1, X09 p2, X10 p3, X11 p4, X12 p5, X13 p6, X14 p7, X15 p8, X16 p9, X17 p10) { } }
class X19 { public X19(X09 p1, X10 p2, X11 p3, X12 p4, X13 p5, X14 p6, X15 p7, X16 p8, X17 p9, X18 p10) { } }
class X20 { public X20(X10 p1, X11 p2, X12 p3, X13 p4, X14 p5, X15 p6, X16 p7, X17 p8, X18 p9, X19 p10) { } }
class X21 { public X21(X11 p1, X12 p2, X13 p3, X14 p4, X15 p5, X16 p6, X17 p7, X18 p8, X19 p9, X20 p10) { } }
class X22 { public X22(X12 p1, X13 p2, X14 p3, X15 p4, X16 p5, X17 p6, X18 p7, X19 p8, X20 p9, X21 p10) { } }
class X23 { public X23(X13 p1, X14 p2, X15 p3, X16 p4, X17 p5, X18 p6, X19 p7, X20 p8, X21 p9, X22 p10) { } }
class X24 { public X24(X14 p1, X15 p2, X16 p3, X17 p4, X18 p5, X19 p6, X20 p7, X21 p8, X22 p9, X23 p10) { } }
class X25 { public X25(X15 p1, X16 p2, X17 p3, X18 p4, X19 p5, X20 p6, X21 p7, X22 p8, X23 p9, X24 p10) { } }
class X26 { public X26(X16 p1, X17 p2, X18 p3, X19 p4, X20 p5, X21 p6, X22 p7, X23 p8, X24 p9, X25 p10) { } }
class X27 { public X27(X17 p1, X18 p2, X19 p3, X20 p4, X21 p5, X22 p6, X23 p7, X24 p8, X25 p9, X26 p10) { } }
class X28 { public X28(X18 p1, X19 p2, X20 p3, X21 p4, X22 p5, X23 p6, X24 p7, X25 p8, X26 p9, X27 p10) { } }
class X29 { public X29(X19 p1, X20 p2, X21 p3, X22 p4, X23 p5, X24 p6, X25 p7, X26 p8, X27 p9, X28 p10) { } }
class X30 { public X30(X20 p1, X21 p2, X22 p3, X23 p4, X24 p5, X25 p6, X26 p7, X27 p8, X28 p9, X29 p10) { } }
class X31 { public X31(X21 p1, X22 p2, X23 p3, X24 p4, X25 p5, X26 p6, X27 p7, X28 p8, X29 p9, X30 p10) { } }
class X32 { public X32(X22 p1, X23 p2, X24 p3, X25 p4, X26 p5, X27 p6, X28 p7, X29 p8, X30 p9, X31 p10) { } }
class X33 { public X33(X23 p1, X24 p2, X25 p3, X26 p4, X27 p5, X28 p6, X29 p7, X30 p8, X31 p9, X32 p10) { } }
class X34 { public X34(X24 p1, X25 p2, X26 p3, X27 p4, X28 p5, X29 p6, X30 p7, X31 p8, X32 p9, X33 p10) { } }
class X35 { public X35(X25 p1, X26 p2, X27 p3, X28 p4, X29 p5, X30 p6, X31 p7, X32 p8, X33 p9, X34 p10) { } }
class X36 { public X36(X26 p1, X27 p2, X28 p3, X29 p4, X30 p5, X31 p6, X32 p7, X33 p8, X34 p9, X35 p10) { } }
class X37 { public X37(X27 p1, X28 p2, X29 p3, X30 p4, X31 p5, X32 p6, X33 p7, X34 p8, X35 p9, X36 p10) { } }
class X38 { public X38(X28 p1, X29 p2, X30 p3, X31 p4, X32 p5, X33 p6, X34 p7, X35 p8, X36 p9, X37 p10) { } }
class X39 { public X39(X29 p1, X30 p2, X31 p3, X32 p4, X33 p5, X34 p6, X35 p7, X36 p8, X37 p9, X38 p10) { } }
class X40 { public X40(X30 p1, X31 p2, X32 p3, X33 p4, X34 p5, X35 p6, X36 p7, X37 p8, X38 p9, X39 p10) { } }
class X41 { public X41(X31 p1, X32 p2, X33 p3, X34 p4, X35 p5, X36 p6, X37 p7, X38 p8, X39 p9, X40 p10) { } }
class X42 { public X42(X32 p1, X33 p2, X34 p3, X35 p4, X36 p5, X37 p6, X38 p7, X39 p8, X40 p9, X41 p10) { } }
class X43 { public X43(X33 p1, X34 p2, X35 p3, X36 p4, X37 p5, X38 p6, X39 p7, X40 p8, X41 p9, X42 p10) { } }
class X44 { public X44(X34 p1, X35 p2, X36 p3, X37 p4, X38 p5, X39 p6, X40 p7, X41 p8, X42 p9, X43 p10) { } }
class X45 { public X45(X35 p1, X36 p2, X37 p3, X38 p4, X39 p5, X40 p6, X41 p7, X42 p8, X43 p9, X44 p10) { } }
class X46 { public X46(X36 p1, X37 p2, X38 p3, X39 p4, X40 p5, X41 p6, X42 p7, X43 p8, X44 p9, X45 p10) { } }
class X47 { public X47(X37 p1, X38 p2, X39 p3, X40 p4, X41 p5, X42 p6, X43 p7, X44 p8, X45 p9, X46 p10) { } }
class X48 { public X48(X38 p1, X39 p2, X40 p3, X41 p4, X42 p5, X43 p6, X44 p7, X45 p8, X46 p9, X47 p10) { } }
class X49 { public X49(X39 p1, X40 p2, X41 p3, X42 p4, X43 p5, X44 p6, X45 p7, X46 p8, X47 p9, X48 p10) { } }
class X50 { public X50(X40 p1, X41 p2, X42 p3, X43 p4, X44 p5, X45 p6, X46 p7, X47 p8, X48 p9, X49 p10) { } }
class X51 { public X51(X41 p1, X42 p2, X43 p3, X44 p4, X45 p5, X46 p6, X47 p7, X48 p8, X49 p9, X50 p10) { } }
class X52 { public X52(X42 p1, X43 p2, X44 p3, X45 p4, X46 p5, X47 p6, X48 p7, X49 p8, X50 p9, X51 p10) { } }
class X53 { public X53(X43 p1, X44 p2, X45 p3, X46 p4, X47 p5, X48 p6, X49 p7, X50 p8, X51 p9, X52 p10) { } }
class X54 { public X54(X44 p1, X45 p2, X46 p3, X47 p4, X48 p5, X49 p6, X50 p7, X51 p8, X52 p9, X53 p10) { } }
class X55 { public X55(X45 p1, X46 p2, X47 p3, X48 p4, X49 p5, X50 p6, X51 p7, X52 p8, X53 p9, X54 p10) { } }
class X56 { public X56(X46 p1, X47 p2, X48 p3, X49 p4, X50 p5, X51 p6, X52 p7, X53 p8, X54 p9, X55 p10) { } }
class X57 { public X57(X47 p1, X48 p2, X49 p3, X50 p4, X51 p5, X52 p6, X53 p7, X54 p8, X55 p9, X56 p10) { } }
class X58 { public X58(X48 p1, X49 p2, X50 p3, X51 p4, X52 p5, X53 p6, X54 p7, X55 p8, X56 p9, X57 p10) { } }
class X59 { public X59(X49 p1, X50 p2, X51 p3, X52 p4, X53 p5, X54 p6, X55 p7, X56 p8, X57 p9, X58 p10) { } }
class X60 { public X60(X50 p1, X51 p2, X52 p3, X53 p4, X54 p5, X55 p6, X56 p7, X57 p8, X58 p9, X59 p10) { } }
class X61 { public X61(X51 p1, X52 p2, X53 p3, X54 p4, X55 p5, X56 p6, X57 p7, X58 p8, X59 p9, X60 p10) { } }
class X62 { public X62(X52 p1, X53 p2, X54 p3, X55 p4, X56 p5, X57 p6, X58 p7, X59 p8, X60 p9, X61 p10) { } }
class X63 { public X63(X53 p1, X54 p2, X55 p3, X56 p4, X57 p5, X58 p6, X59 p7, X60 p8, X61 p9, X62 p10) { } }
class X64 { public X64(X54 p1, X55 p2, X56 p3, X57 p4, X58 p5, X59 p6, X60 p7, X61 p8, X62 p9, X63 p10) { } }
class X65 { public X65(X55 p1, X56 p2, X57 p3, X58 p4, X59 p5, X60 p6, X61 p7, X62 p8, X63 p9, X64 p10) { } }
class X66 { public X66(X56 p1, X57 p2, X58 p3, X59 p4, X60 p5, X61 p6, X62 p7, X63 p8, X64 p9, X65 p10) { } }
class X67 { public X67(X57 p1, X58 p2, X59 p3, X60 p4, X61 p5, X62 p6, X63 p7, X64 p8, X65 p9, X66 p10) { } }
class X68 { public X68(X58 p1, X59 p2, X60 p3, X61 p4, X62 p5, X63 p6, X64 p7, X65 p8, X66 p9, X67 p10) { } }
class X69 { public X69(X59 p1, X60 p2, X61 p3, X62 p4, X63 p5, X64 p6, X65 p7, X66 p8, X67 p9, X68 p10) { } }
class X70 { public X70(X60 p1, X61 p2, X62 p3, X63 p4, X64 p5, X65 p6, X66 p7, X67 p8, X68 p9, X69 p10) { } }
class X71 { public X71(X61 p1, X62 p2, X63 p3, X64 p4, X65 p5, X66 p6, X67 p7, X68 p8, X69 p9, X70 p10) { } }
class X72 { public X72(X62 p1, X63 p2, X64 p3, X65 p4, X66 p5, X67 p6, X68 p7, X69 p8, X70 p9, X71 p10) { } }
class X73 { public X73(X63 p1, X64 p2, X65 p3, X66 p4, X67 p5, X68 p6, X69 p7, X70 p8, X71 p9, X72 p10) { } }
class X74 { public X74(X64 p1, X65 p2, X66 p3, X67 p4, X68 p5, X69 p6, X70 p7, X71 p8, X72 p9, X73 p10) { } }
class X75 { public X75(X65 p1, X66 p2, X67 p3, X68 p4, X69 p5, X70 p6, X71 p7, X72 p8, X73 p9, X74 p10) { } }
class X76 { public X76(X66 p1, X67 p2, X68 p3, X69 p4, X70 p5, X71 p6, X72 p7, X73 p8, X74 p9, X75 p10) { } }
class X77 { public X77(X67 p1, X68 p2, X69 p3, X70 p4, X71 p5, X72 p6, X73 p7, X74 p8, X75 p9, X76 p10) { } }
class X78 { public X78(X68 p1, X69 p2, X70 p3, X71 p4, X72 p5, X73 p6, X74 p7, X75 p8, X76 p9, X77 p10) { } }
class X79 { public X79(X69 p1, X70 p2, X71 p3, X72 p4, X73 p5, X74 p6, X75 p7, X76 p8, X77 p9, X78 p10) { } }
class X80 { public X80(X70 p1, X71 p2, X72 p3, X73 p4, X74 p5, X75 p6, X76 p7, X77 p8, X78 p9, X79 p10) { } }
class X81 { public X81(X71 p1, X72 p2, X73 p3, X74 p4, X75 p5, X76 p6, X77 p7, X78 p8, X79 p9, X80 p10) { } }
class X82 { public X82(X72 p1, X73 p2, X74 p3, X75 p4, X76 p5, X77 p6, X78 p7, X79 p8, X80 p9, X81 p10) { } }
class X83 { public X83(X73 p1, X74 p2, X75 p3, X76 p4, X77 p5, X78 p6, X79 p7, X80 p8, X81 p9, X82 p10) { } }
class X84 { public X84(X74 p1, X75 p2, X76 p3, X77 p4, X78 p5, X79 p6, X80 p7, X81 p8, X82 p9, X83 p10) { } }
class X85 { public X85(X75 p1, X76 p2, X77 p3, X78 p4, X79 p5, X80 p6, X81 p7, X82 p8, X83 p9, X84 p10) { } }
class X86 { public X86(X76 p1, X77 p2, X78 p3, X79 p4, X80 p5, X81 p6, X82 p7, X83 p8, X84 p9, X85 p10) { } }
class X87 { public X87(X77 p1, X78 p2, X79 p3, X80 p4, X81 p5, X82 p6, X83 p7, X84 p8, X85 p9, X86 p10) { } }
class X88 { public X88(X78 p1, X79 p2, X80 p3, X81 p4, X82 p5, X83 p6, X84 p7, X85 p8, X86 p9, X87 p10) { } }
class X89 { public X89(X79 p1, X80 p2, X81 p3, X82 p4, X83 p5, X84 p6, X85 p7, X86 p8, X87 p9, X88 p10) { } }
class X90 { public X90(X80 p1, X81 p2, X82 p3, X83 p4, X84 p5, X85 p6, X86 p7, X87 p8, X88 p9, X89 p10) { } }
class X91 { public X91(X81 p1, X82 p2, X83 p3, X84 p4, X85 p5, X86 p6, X87 p7, X88 p8, X89 p9, X90 p10) { } }
class X92 { public X92(X82 p1, X83 p2, X84 p3, X85 p4, X86 p5, X87 p6, X88 p7, X89 p8, X90 p9, X91 p10) { } }
class X93 { public X93(X83 p1, X84 p2, X85 p3, X86 p4, X87 p5, X88 p6, X89 p7, X90 p8, X91 p9, X92 p10) { } }
class X94 { public X94(X84 p1, X85 p2, X86 p3, X87 p4, X88 p5, X89 p6, X90 p7, X91 p8, X92 p9, X93 p10) { } }
class X95 { public X95(X85 p1, X86 p2, X87 p3, X88 p4, X89 p5, X90 p6, X91 p7, X92 p8, X93 p9, X94 p10) { } }
class X96 { public X96(X86 p1, X87 p2, X88 p3, X89 p4, X90 p5, X91 p6, X92 p7, X93 p8, X94 p9, X95 p10) { } }
class X97 { public X97(X87 p1, X88 p2, X89 p3, X90 p4, X91 p5, X92 p6, X93 p7, X94 p8, X95 p9, X96 p10) { } }
class X98 { public X98(X88 p1, X89 p2, X90 p3, X91 p4, X92 p5, X93 p6, X94 p7, X95 p8, X96 p9, X97 p10) { } }
class X99 { public X99(X89 p1, X90 p2, X91 p3, X92 p4, X93 p5, X94 p6, X95 p7, X96 p8, X97 p9, X98 p10) { } }
interface I00 { void dummy(); }; class Impl00 : I00 { public Impl00(X00 p1, X01 p2, X02 p3, X03 p4, X04 p5, X05 p6, X06 p7, X07 p8, X08 p9, X09 p10) { } void I00.dummy() { } }
interface I01 { void dummy(); }; class Impl01 : I01 { public Impl01(X01 p1, X02 p2, X03 p3, X04 p4, X05 p5, X06 p6, X07 p7, X08 p8, X09 p9, X10 p10) { } void I01.dummy() { } }
interface I02 { void dummy(); }; class Impl02 : I02 { public Impl02(X02 p1, X03 p2, X04 p3, X05 p4, X06 p5, X07 p6, X08 p7, X09 p8, X10 p9, X11 p10) { } void I02.dummy() { } }
interface I03 { void dummy(); }; class Impl03 : I03 { public Impl03(X03 p1, X04 p2, X05 p3, X06 p4, X07 p5, X08 p6, X09 p7, X10 p8, X11 p9, X12 p10) { } void I03.dummy() { } }
interface I04 { void dummy(); }; class Impl04 : I04 { public Impl04(X04 p1, X05 p2, X06 p3, X07 p4, X08 p5, X09 p6, X10 p7, X11 p8, X12 p9, X13 p10) { } void I04.dummy() { } }
interface I05 { void dummy(); }; class Impl05 : I05 { public Impl05(X05 p1, X06 p2, X07 p3, X08 p4, X09 p5, X10 p6, X11 p7, X12 p8, X13 p9, X14 p10) { } void I05.dummy() { } }
interface I06 { void dummy(); }; class Impl06 : I06 { public Impl06(X06 p1, X07 p2, X08 p3, X09 p4, X10 p5, X11 p6, X12 p7, X13 p8, X14 p9, X15 p10) { } void I06.dummy() { } }
interface I07 { void dummy(); }; class Impl07 : I07 { public Impl07(X07 p1, X08 p2, X09 p3, X10 p4, X11 p5, X12 p6, X13 p7, X14 p8, X15 p9, X16 p10) { } void I07.dummy() { } }
interface I08 { void dummy(); }; class Impl08 : I08 { public Impl08(X08 p1, X09 p2, X10 p3, X11 p4, X12 p5, X13 p6, X14 p7, X15 p8, X16 p9, X17 p10) { } void I08.dummy() { } }
interface I09 { void dummy(); }; class Impl09 : I09 { public Impl09(X09 p1, X10 p2, X11 p3, X12 p4, X13 p5, X14 p6, X15 p7, X16 p8, X17 p9, X18 p10) { } void I09.dummy() { } }
interface I10 { void dummy(); }; class Impl10 : I10 { public Impl10(X10 p1, X11 p2, X12 p3, X13 p4, X14 p5, X15 p6, X16 p7, X17 p8, X18 p9, X19 p10) { } void I10.dummy() { } }
interface I11 { void dummy(); }; class Impl11 : I11 { public Impl11(X11 p1, X12 p2, X13 p3, X14 p4, X15 p5, X16 p6, X17 p7, X18 p8, X19 p9, X20 p10) { } void I11.dummy() { } }
interface I12 { void dummy(); }; class Impl12 : I12 { public Impl12(X12 p1, X13 p2, X14 p3, X15 p4, X16 p5, X17 p6, X18 p7, X19 p8, X20 p9, X21 p10) { } void I12.dummy() { } }
interface I13 { void dummy(); }; class Impl13 : I13 { public Impl13(X13 p1, X14 p2, X15 p3, X16 p4, X17 p5, X18 p6, X19 p7, X20 p8, X21 p9, X22 p10) { } void I13.dummy() { } }
interface I14 { void dummy(); }; class Impl14 : I14 { public Impl14(X14 p1, X15 p2, X16 p3, X17 p4, X18 p5, X19 p6, X20 p7, X21 p8, X22 p9, X23 p10) { } void I14.dummy() { } }
interface I15 { void dummy(); }; class Impl15 : I15 { public Impl15(X15 p1, X16 p2, X17 p3, X18 p4, X19 p5, X20 p6, X21 p7, X22 p8, X23 p9, X24 p10) { } void I15.dummy() { } }
interface I16 { void dummy(); }; class Impl16 : I16 { public Impl16(X16 p1, X17 p2, X18 p3, X19 p4, X20 p5, X21 p6, X22 p7, X23 p8, X24 p9, X25 p10) { } void I16.dummy() { } }
interface I17 { void dummy(); }; class Impl17 : I17 { public Impl17(X17 p1, X18 p2, X19 p3, X20 p4, X21 p5, X22 p6, X23 p7, X24 p8, X25 p9, X26 p10) { } void I17.dummy() { } }
interface I18 { void dummy(); }; class Impl18 : I18 { public Impl18(X18 p1, X19 p2, X20 p3, X21 p4, X22 p5, X23 p6, X24 p7, X25 p8, X26 p9, X27 p10) { } void I18.dummy() { } }
interface I19 { void dummy(); }; class Impl19 : I19 { public Impl19(X19 p1, X20 p2, X21 p3, X22 p4, X23 p5, X24 p6, X25 p7, X26 p8, X27 p9, X28 p10) { } void I19.dummy() { } }
interface I20 { void dummy(); }; class Impl20 : I20 { public Impl20(X20 p1, X21 p2, X22 p3, X23 p4, X24 p5, X25 p6, X26 p7, X27 p8, X28 p9, X29 p10) { } void I20.dummy() { } }
interface I21 { void dummy(); }; class Impl21 : I21 { public Impl21(X21 p1, X22 p2, X23 p3, X24 p4, X25 p5, X26 p6, X27 p7, X28 p8, X29 p9, X30 p10) { } void I21.dummy() { } }
interface I22 { void dummy(); }; class Impl22 : I22 { public Impl22(X22 p1, X23 p2, X24 p3, X25 p4, X26 p5, X27 p6, X28 p7, X29 p8, X30 p9, X31 p10) { } void I22.dummy() { } }
interface I23 { void dummy(); }; class Impl23 : I23 { public Impl23(X23 p1, X24 p2, X25 p3, X26 p4, X27 p5, X28 p6, X29 p7, X30 p8, X31 p9, X32 p10) { } void I23.dummy() { } }
interface I24 { void dummy(); }; class Impl24 : I24 { public Impl24(X24 p1, X25 p2, X26 p3, X27 p4, X28 p5, X29 p6, X30 p7, X31 p8, X32 p9, X33 p10) { } void I24.dummy() { } }
interface I25 { void dummy(); }; class Impl25 : I25 { public Impl25(X25 p1, X26 p2, X27 p3, X28 p4, X29 p5, X30 p6, X31 p7, X32 p8, X33 p9, X34 p10) { } void I25.dummy() { } }
interface I26 { void dummy(); }; class Impl26 : I26 { public Impl26(X26 p1, X27 p2, X28 p3, X29 p4, X30 p5, X31 p6, X32 p7, X33 p8, X34 p9, X35 p10) { } void I26.dummy() { } }
interface I27 { void dummy(); }; class Impl27 : I27 { public Impl27(X27 p1, X28 p2, X29 p3, X30 p4, X31 p5, X32 p6, X33 p7, X34 p8, X35 p9, X36 p10) { } void I27.dummy() { } }
interface I28 { void dummy(); }; class Impl28 : I28 { public Impl28(X28 p1, X29 p2, X30 p3, X31 p4, X32 p5, X33 p6, X34 p7, X35 p8, X36 p9, X37 p10) { } void I28.dummy() { } }
interface I29 { void dummy(); }; class Impl29 : I29 { public Impl29(X29 p1, X30 p2, X31 p3, X32 p4, X33 p5, X34 p6, X35 p7, X36 p8, X37 p9, X38 p10) { } void I29.dummy() { } }
interface I30 { void dummy(); }; class Impl30 : I30 { public Impl30(X30 p1, X31 p2, X32 p3, X33 p4, X34 p5, X35 p6, X36 p7, X37 p8, X38 p9, X39 p10) { } void I30.dummy() { } }
interface I31 { void dummy(); }; class Impl31 : I31 { public Impl31(X31 p1, X32 p2, X33 p3, X34 p4, X35 p5, X36 p6, X37 p7, X38 p8, X39 p9, X40 p10) { } void I31.dummy() { } }
interface I32 { void dummy(); }; class Impl32 : I32 { public Impl32(X32 p1, X33 p2, X34 p3, X35 p4, X36 p5, X37 p6, X38 p7, X39 p8, X40 p9, X41 p10) { } void I32.dummy() { } }
interface I33 { void dummy(); }; class Impl33 : I33 { public Impl33(X33 p1, X34 p2, X35 p3, X36 p4, X37 p5, X38 p6, X39 p7, X40 p8, X41 p9, X42 p10) { } void I33.dummy() { } }
interface I34 { void dummy(); }; class Impl34 : I34 { public Impl34(X34 p1, X35 p2, X36 p3, X37 p4, X38 p5, X39 p6, X40 p7, X41 p8, X42 p9, X43 p10) { } void I34.dummy() { } }
interface I35 { void dummy(); }; class Impl35 : I35 { public Impl35(X35 p1, X36 p2, X37 p3, X38 p4, X39 p5, X40 p6, X41 p7, X42 p8, X43 p9, X44 p10) { } void I35.dummy() { } }
interface I36 { void dummy(); }; class Impl36 : I36 { public Impl36(X36 p1, X37 p2, X38 p3, X39 p4, X40 p5, X41 p6, X42 p7, X43 p8, X44 p9, X45 p10) { } void I36.dummy() { } }
interface I37 { void dummy(); }; class Impl37 : I37 { public Impl37(X37 p1, X38 p2, X39 p3, X40 p4, X41 p5, X42 p6, X43 p7, X44 p8, X45 p9, X46 p10) { } void I37.dummy() { } }
interface I38 { void dummy(); }; class Impl38 : I38 { public Impl38(X38 p1, X39 p2, X40 p3, X41 p4, X42 p5, X43 p6, X44 p7, X45 p8, X46 p9, X47 p10) { } void I38.dummy() { } }
interface I39 { void dummy(); }; class Impl39 : I39 { public Impl39(X39 p1, X40 p2, X41 p3, X42 p4, X43 p5, X44 p6, X45 p7, X46 p8, X47 p9, X48 p10) { } void I39.dummy() { } }
interface I40 { void dummy(); }; class Impl40 : I40 { public Impl40(X40 p1, X41 p2, X42 p3, X43 p4, X44 p5, X45 p6, X46 p7, X47 p8, X48 p9, X49 p10) { } void I40.dummy() { } }
interface I41 { void dummy(); }; class Impl41 : I41 { public Impl41(X41 p1, X42 p2, X43 p3, X44 p4, X45 p5, X46 p6, X47 p7, X48 p8, X49 p9, X50 p10) { } void I41.dummy() { } }
interface I42 { void dummy(); }; class Impl42 : I42 { public Impl42(X42 p1, X43 p2, X44 p3, X45 p4, X46 p5, X47 p6, X48 p7, X49 p8, X50 p9, X51 p10) { } void I42.dummy() { } }
interface I43 { void dummy(); }; class Impl43 : I43 { public Impl43(X43 p1, X44 p2, X45 p3, X46 p4, X47 p5, X48 p6, X49 p7, X50 p8, X51 p9, X52 p10) { } void I43.dummy() { } }
interface I44 { void dummy(); }; class Impl44 : I44 { public Impl44(X44 p1, X45 p2, X46 p3, X47 p4, X48 p5, X49 p6, X50 p7, X51 p8, X52 p9, X53 p10) { } void I44.dummy() { } }
interface I45 { void dummy(); }; class Impl45 : I45 { public Impl45(X45 p1, X46 p2, X47 p3, X48 p4, X49 p5, X50 p6, X51 p7, X52 p8, X53 p9, X54 p10) { } void I45.dummy() { } }
interface I46 { void dummy(); }; class Impl46 : I46 { public Impl46(X46 p1, X47 p2, X48 p3, X49 p4, X50 p5, X51 p6, X52 p7, X53 p8, X54 p9, X55 p10) { } void I46.dummy() { } }
interface I47 { void dummy(); }; class Impl47 : I47 { public Impl47(X47 p1, X48 p2, X49 p3, X50 p4, X51 p5, X52 p6, X53 p7, X54 p8, X55 p9, X56 p10) { } void I47.dummy() { } }
interface I48 { void dummy(); }; class Impl48 : I48 { public Impl48(X48 p1, X49 p2, X50 p3, X51 p4, X52 p5, X53 p6, X54 p7, X55 p8, X56 p9, X57 p10) { } void I48.dummy() { } }
interface I49 { void dummy(); }; class Impl49 : I49 { public Impl49(X49 p1, X50 p2, X51 p3, X52 p4, X53 p5, X54 p6, X55 p7, X56 p8, X57 p9, X58 p10) { } void I49.dummy() { } }
interface I50 { void dummy(); }; class Impl50 : I50 { public Impl50(X50 p1, X51 p2, X52 p3, X53 p4, X54 p5, X55 p6, X56 p7, X57 p8, X58 p9, X59 p10) { } void I50.dummy() { } }
interface I51 { void dummy(); }; class Impl51 : I51 { public Impl51(X51 p1, X52 p2, X53 p3, X54 p4, X55 p5, X56 p6, X57 p7, X58 p8, X59 p9, X60 p10) { } void I51.dummy() { } }
interface I52 { void dummy(); }; class Impl52 : I52 { public Impl52(X52 p1, X53 p2, X54 p3, X55 p4, X56 p5, X57 p6, X58 p7, X59 p8, X60 p9, X61 p10) { } void I52.dummy() { } }
interface I53 { void dummy(); }; class Impl53 : I53 { public Impl53(X53 p1, X54 p2, X55 p3, X56 p4, X57 p5, X58 p6, X59 p7, X60 p8, X61 p9, X62 p10) { } void I53.dummy() { } }
interface I54 { void dummy(); }; class Impl54 : I54 { public Impl54(X54 p1, X55 p2, X56 p3, X57 p4, X58 p5, X59 p6, X60 p7, X61 p8, X62 p9, X63 p10) { } void I54.dummy() { } }
interface I55 { void dummy(); }; class Impl55 : I55 { public Impl55(X55 p1, X56 p2, X57 p3, X58 p4, X59 p5, X60 p6, X61 p7, X62 p8, X63 p9, X64 p10) { } void I55.dummy() { } }
interface I56 { void dummy(); }; class Impl56 : I56 { public Impl56(X56 p1, X57 p2, X58 p3, X59 p4, X60 p5, X61 p6, X62 p7, X63 p8, X64 p9, X65 p10) { } void I56.dummy() { } }
interface I57 { void dummy(); }; class Impl57 : I57 { public Impl57(X57 p1, X58 p2, X59 p3, X60 p4, X61 p5, X62 p6, X63 p7, X64 p8, X65 p9, X66 p10) { } void I57.dummy() { } }
interface I58 { void dummy(); }; class Impl58 : I58 { public Impl58(X58 p1, X59 p2, X60 p3, X61 p4, X62 p5, X63 p6, X64 p7, X65 p8, X66 p9, X67 p10) { } void I58.dummy() { } }
interface I59 { void dummy(); }; class Impl59 : I59 { public Impl59(X59 p1, X60 p2, X61 p3, X62 p4, X63 p5, X64 p6, X65 p7, X66 p8, X67 p9, X68 p10) { } void I59.dummy() { } }
interface I60 { void dummy(); }; class Impl60 : I60 { public Impl60(X60 p1, X61 p2, X62 p3, X63 p4, X64 p5, X65 p6, X66 p7, X67 p8, X68 p9, X69 p10) { } void I60.dummy() { } }
interface I61 { void dummy(); }; class Impl61 : I61 { public Impl61(X61 p1, X62 p2, X63 p3, X64 p4, X65 p5, X66 p6, X67 p7, X68 p8, X69 p9, X70 p10) { } void I61.dummy() { } }
interface I62 { void dummy(); }; class Impl62 : I62 { public Impl62(X62 p1, X63 p2, X64 p3, X65 p4, X66 p5, X67 p6, X68 p7, X69 p8, X70 p9, X71 p10) { } void I62.dummy() { } }
interface I63 { void dummy(); }; class Impl63 : I63 { public Impl63(X63 p1, X64 p2, X65 p3, X66 p4, X67 p5, X68 p6, X69 p7, X70 p8, X71 p9, X72 p10) { } void I63.dummy() { } }
interface I64 { void dummy(); }; class Impl64 : I64 { public Impl64(X64 p1, X65 p2, X66 p3, X67 p4, X68 p5, X69 p6, X70 p7, X71 p8, X72 p9, X73 p10) { } void I64.dummy() { } }
interface I65 { void dummy(); }; class Impl65 : I65 { public Impl65(X65 p1, X66 p2, X67 p3, X68 p4, X69 p5, X70 p6, X71 p7, X72 p8, X73 p9, X74 p10) { } void I65.dummy() { } }
interface I66 { void dummy(); }; class Impl66 : I66 { public Impl66(X66 p1, X67 p2, X68 p3, X69 p4, X70 p5, X71 p6, X72 p7, X73 p8, X74 p9, X75 p10) { } void I66.dummy() { } }
interface I67 { void dummy(); }; class Impl67 : I67 { public Impl67(X67 p1, X68 p2, X69 p3, X70 p4, X71 p5, X72 p6, X73 p7, X74 p8, X75 p9, X76 p10) { } void I67.dummy() { } }
interface I68 { void dummy(); }; class Impl68 : I68 { public Impl68(X68 p1, X69 p2, X70 p3, X71 p4, X72 p5, X73 p6, X74 p7, X75 p8, X76 p9, X77 p10) { } void I68.dummy() { } }
interface I69 { void dummy(); }; class Impl69 : I69 { public Impl69(X69 p1, X70 p2, X71 p3, X72 p4, X73 p5, X74 p6, X75 p7, X76 p8, X77 p9, X78 p10) { } void I69.dummy() { } }
interface I70 { void dummy(); }; class Impl70 : I70 { public Impl70(X70 p1, X71 p2, X72 p3, X73 p4, X74 p5, X75 p6, X76 p7, X77 p8, X78 p9, X79 p10) { } void I70.dummy() { } }
interface I71 { void dummy(); }; class Impl71 : I71 { public Impl71(X71 p1, X72 p2, X73 p3, X74 p4, X75 p5, X76 p6, X77 p7, X78 p8, X79 p9, X80 p10) { } void I71.dummy() { } }
interface I72 { void dummy(); }; class Impl72 : I72 { public Impl72(X72 p1, X73 p2, X74 p3, X75 p4, X76 p5, X77 p6, X78 p7, X79 p8, X80 p9, X81 p10) { } void I72.dummy() { } }
interface I73 { void dummy(); }; class Impl73 : I73 { public Impl73(X73 p1, X74 p2, X75 p3, X76 p4, X77 p5, X78 p6, X79 p7, X80 p8, X81 p9, X82 p10) { } void I73.dummy() { } }
interface I74 { void dummy(); }; class Impl74 : I74 { public Impl74(X74 p1, X75 p2, X76 p3, X77 p4, X78 p5, X79 p6, X80 p7, X81 p8, X82 p9, X83 p10) { } void I74.dummy() { } }
interface I75 { void dummy(); }; class Impl75 : I75 { public Impl75(X75 p1, X76 p2, X77 p3, X78 p4, X79 p5, X80 p6, X81 p7, X82 p8, X83 p9, X84 p10) { } void I75.dummy() { } }
interface I76 { void dummy(); }; class Impl76 : I76 { public Impl76(X76 p1, X77 p2, X78 p3, X79 p4, X80 p5, X81 p6, X82 p7, X83 p8, X84 p9, X85 p10) { } void I76.dummy() { } }
interface I77 { void dummy(); }; class Impl77 : I77 { public Impl77(X77 p1, X78 p2, X79 p3, X80 p4, X81 p5, X82 p6, X83 p7, X84 p8, X85 p9, X86 p10) { } void I77.dummy() { } }
interface I78 { void dummy(); }; class Impl78 : I78 { public Impl78(X78 p1, X79 p2, X80 p3, X81 p4, X82 p5, X83 p6, X84 p7, X85 p8, X86 p9, X87 p10) { } void I78.dummy() { } }
interface I79 { void dummy(); }; class Impl79 : I79 { public Impl79(X79 p1, X80 p2, X81 p3, X82 p4, X83 p5, X84 p6, X85 p7, X86 p8, X87 p9, X88 p10) { } void I79.dummy() { } }
interface I80 { void dummy(); }; class Impl80 : I80 { public Impl80(X80 p1, X81 p2, X82 p3, X83 p4, X84 p5, X85 p6, X86 p7, X87 p8, X88 p9, X89 p10) { } void I80.dummy() { } }
interface I81 { void dummy(); }; class Impl81 : I81 { public Impl81(X81 p1, X82 p2, X83 p3, X84 p4, X85 p5, X86 p6, X87 p7, X88 p8, X89 p9, X90 p10) { } void I81.dummy() { } }
interface I82 { void dummy(); }; class Impl82 : I82 { public Impl82(X82 p1, X83 p2, X84 p3, X85 p4, X86 p5, X87 p6, X88 p7, X89 p8, X90 p9, X91 p10) { } void I82.dummy() { } }
interface I83 { void dummy(); }; class Impl83 : I83 { public Impl83(X83 p1, X84 p2, X85 p3, X86 p4, X87 p5, X88 p6, X89 p7, X90 p8, X91 p9, X92 p10) { } void I83.dummy() { } }
interface I84 { void dummy(); }; class Impl84 : I84 { public Impl84(X84 p1, X85 p2, X86 p3, X87 p4, X88 p5, X89 p6, X90 p7, X91 p8, X92 p9, X93 p10) { } void I84.dummy() { } }
interface I85 { void dummy(); }; class Impl85 : I85 { public Impl85(X85 p1, X86 p2, X87 p3, X88 p4, X89 p5, X90 p6, X91 p7, X92 p8, X93 p9, X94 p10) { } void I85.dummy() { } }
interface I86 { void dummy(); }; class Impl86 : I86 { public Impl86(X86 p1, X87 p2, X88 p3, X89 p4, X90 p5, X91 p6, X92 p7, X93 p8, X94 p9, X95 p10) { } void I86.dummy() { } }
interface I87 { void dummy(); }; class Impl87 : I87 { public Impl87(X87 p1, X88 p2, X89 p3, X90 p4, X91 p5, X92 p6, X93 p7, X94 p8, X95 p9, X96 p10) { } void I87.dummy() { } }
interface I88 { void dummy(); }; class Impl88 : I88 { public Impl88(X88 p1, X89 p2, X90 p3, X91 p4, X92 p5, X93 p6, X94 p7, X95 p8, X96 p9, X97 p10) { } void I88.dummy() { } }
interface I89 { void dummy(); }; class Impl89 : I89 { public Impl89(X89 p1, X90 p2, X91 p3, X92 p4, X93 p5, X94 p6, X95 p7, X96 p8, X97 p9, X98 p10) { } void I89.dummy() { } }
interface I90 { void dummy(); }; class Impl90 : I90 { public Impl90(X90 p1, X91 p2, X92 p3, X93 p4, X94 p5, X95 p6, X96 p7, X97 p8, X98 p9, X99 p10) { } void I90.dummy() { } }
interface I91 { void dummy(); }; class Impl91 : I91 { public Impl91(X91 p1, X92 p2, X93 p3, X94 p4, X95 p5, X96 p6, X97 p7, X98 p8, X99 p9, X00 p10) { } void I91.dummy() { } }
interface I92 { void dummy(); }; class Impl92 : I92 { public Impl92(X92 p1, X93 p2, X94 p3, X95 p4, X96 p5, X97 p6, X98 p7, X99 p8, X00 p9, X01 p10) { } void I92.dummy() { } }
interface I93 { void dummy(); }; class Impl93 : I93 { public Impl93(X93 p1, X94 p2, X95 p3, X96 p4, X97 p5, X98 p6, X99 p7, X00 p8, X01 p9, X02 p10) { } void I93.dummy() { } }
interface I94 { void dummy(); }; class Impl94 : I94 { public Impl94(X94 p1, X95 p2, X96 p3, X97 p4, X98 p5, X99 p6, X00 p7, X01 p8, X02 p9, X03 p10) { } void I94.dummy() { } }
interface I95 { void dummy(); }; class Impl95 : I95 { public Impl95(X95 p1, X96 p2, X97 p3, X98 p4, X99 p5, X00 p6, X01 p7, X02 p8, X03 p9, X04 p10) { } void I95.dummy() { } }
interface I96 { void dummy(); }; class Impl96 : I96 { public Impl96(X96 p1, X97 p2, X98 p3, X99 p4, X00 p5, X01 p6, X02 p7, X03 p8, X04 p9, X05 p10) { } void I96.dummy() { } }
interface I97 { void dummy(); }; class Impl97 : I97 { public Impl97(X97 p1, X98 p2, X99 p3, X00 p4, X01 p5, X02 p6, X03 p7, X04 p8, X05 p9, X06 p10) { } void I97.dummy() { } }
interface I98 { void dummy(); }; class Impl98 : I98 { public Impl98(X98 p1, X99 p2, X00 p3, X01 p4, X02 p5, X03 p6, X04 p7, X05 p8, X06 p9, X07 p10) { } void I98.dummy() { } }
interface I99 { void dummy(); }; class Impl99 : I99 { public Impl99(X99 p1, X00 p2, X01 p3, X02 p4, X03 p5, X04 p6, X05 p7, X06 p8, X07 p9, X08 p10) { } void I99.dummy() { } }
class C0 { public C0(I00 p1, I01 p2, I02 p3, I03 p4, I04 p5, I05 p6, I06 p7, I07 p8, I08 p9, I09 p10) { } }
class C1 { public C1(I10 p1, I11 p2, I12 p3, I13 p4, I14 p5, I15 p6, I16 p7, I17 p8, I18 p9, I19 p10) { } }
class C2 { public C2(I20 p1, I21 p2, I22 p3, I23 p4, I24 p5, I25 p6, I26 p7, I27 p8, I28 p9, I29 p10) { } }
class C3 { public C3(I30 p1, I31 p2, I32 p3, I33 p4, I34 p5, I35 p6, I36 p7, I37 p8, I38 p9, I39 p10) { } }
class C4 { public C4(I40 p1, I41 p2, I42 p3, I43 p4, I44 p5, I45 p6, I46 p7, I47 p8, I48 p9, I49 p10) { } }
class C5 { public C5(I50 p1, I51 p2, I52 p3, I53 p4, I54 p5, I55 p6, I56 p7, I57 p8, I58 p9, I59 p10) { } }
class C6 { public C6(I60 p1, I61 p2, I62 p3, I63 p4, I64 p5, I65 p6, I66 p7, I67 p8, I68 p9, I69 p10) { } }
class C7 { public C7(I70 p1, I71 p2, I72 p3, I73 p4, I74 p5, I75 p6, I76 p7, I77 p8, I78 p9, I79 p10) { } }
class C8 { public C8(I80 p1, I81 p2, I82 p3, I83 p4, I84 p5, I85 p6, I86 p7, I87 p8, I88 p9, I89 p10) { } }
class C9 { public C9(I90 p1, I91 p2, I92 p3, I93 p4, I94 p5, I95 p6, I96 p7, I97 p8, I98 p9, I99 p10) { } }
class Complex { public Complex(C0 p1, C1 p2, C2 p3, C3 p4, C4 p5, C5 p6, C6 p7, C7 p8, C8 p9, C9 p10) { } }

class Module : Ninject.Modules.NinjectModule {
  public override void Load() {
    Bind<I00>().To<Impl00>();
    Bind<I01>().To<Impl01>();
    Bind<I02>().To<Impl02>();
    Bind<I03>().To<Impl03>();
    Bind<I04>().To<Impl04>();
    Bind<I05>().To<Impl05>();
    Bind<I06>().To<Impl06>();
    Bind<I07>().To<Impl07>();
    Bind<I08>().To<Impl08>();
    Bind<I09>().To<Impl09>();
    Bind<I10>().To<Impl10>();
    Bind<I11>().To<Impl11>();
    Bind<I12>().To<Impl12>();
    Bind<I13>().To<Impl13>();
    Bind<I14>().To<Impl14>();
    Bind<I15>().To<Impl15>();
    Bind<I16>().To<Impl16>();
    Bind<I17>().To<Impl17>();
    Bind<I18>().To<Impl18>();
    Bind<I19>().To<Impl19>();
    Bind<I20>().To<Impl20>();
    Bind<I21>().To<Impl21>();
    Bind<I22>().To<Impl22>();
    Bind<I23>().To<Impl23>();
    Bind<I24>().To<Impl24>();
    Bind<I25>().To<Impl25>();
    Bind<I26>().To<Impl26>();
    Bind<I27>().To<Impl27>();
    Bind<I28>().To<Impl28>();
    Bind<I29>().To<Impl29>();
    Bind<I30>().To<Impl30>();
    Bind<I31>().To<Impl31>();
    Bind<I32>().To<Impl32>();
    Bind<I33>().To<Impl33>();
    Bind<I34>().To<Impl34>();
    Bind<I35>().To<Impl35>();
    Bind<I36>().To<Impl36>();
    Bind<I37>().To<Impl37>();
    Bind<I38>().To<Impl38>();
    Bind<I39>().To<Impl39>();
    Bind<I40>().To<Impl40>();
    Bind<I41>().To<Impl41>();
    Bind<I42>().To<Impl42>();
    Bind<I43>().To<Impl43>();
    Bind<I44>().To<Impl44>();
    Bind<I45>().To<Impl45>();
    Bind<I46>().To<Impl46>();
    Bind<I47>().To<Impl47>();
    Bind<I48>().To<Impl48>();
    Bind<I49>().To<Impl49>();
    Bind<I50>().To<Impl50>();
    Bind<I51>().To<Impl51>();
    Bind<I52>().To<Impl52>();
    Bind<I53>().To<Impl53>();
    Bind<I54>().To<Impl54>();
    Bind<I55>().To<Impl55>();
    Bind<I56>().To<Impl56>();
    Bind<I57>().To<Impl57>();
    Bind<I58>().To<Impl58>();
    Bind<I59>().To<Impl59>();
    Bind<I60>().To<Impl60>();
    Bind<I61>().To<Impl61>();
    Bind<I62>().To<Impl62>();
    Bind<I63>().To<Impl63>();
    Bind<I64>().To<Impl64>();
    Bind<I65>().To<Impl65>();
    Bind<I66>().To<Impl66>();
    Bind<I67>().To<Impl67>();
    Bind<I68>().To<Impl68>();
    Bind<I69>().To<Impl69>();
    Bind<I70>().To<Impl70>();
    Bind<I71>().To<Impl71>();
    Bind<I72>().To<Impl72>();
    Bind<I73>().To<Impl73>();
    Bind<I74>().To<Impl74>();
    Bind<I75>().To<Impl75>();
    Bind<I76>().To<Impl76>();
    Bind<I77>().To<Impl77>();
    Bind<I78>().To<Impl78>();
    Bind<I79>().To<Impl79>();
    Bind<I80>().To<Impl80>();
    Bind<I81>().To<Impl81>();
    Bind<I82>().To<Impl82>();
    Bind<I83>().To<Impl83>();
    Bind<I84>().To<Impl84>();
    Bind<I85>().To<Impl85>();
    Bind<I86>().To<Impl86>();
    Bind<I87>().To<Impl87>();
    Bind<I88>().To<Impl88>();
    Bind<I89>().To<Impl89>();
    Bind<I90>().To<Impl90>();
    Bind<I91>().To<Impl91>();
    Bind<I92>().To<Impl92>();
    Bind<I93>().To<Impl93>();
    Bind<I94>().To<Impl94>();
    Bind<I95>().To<Impl95>();
    Bind<I96>().To<Impl96>();
    Bind<I97>().To<Impl97>();
    Bind<I98>().To<Impl98>();
    Bind<I99>().To<Impl99>();
  }
}

class ninject {
  static void Main(string[] args) {
    Ninject.IKernel kernel = new StandardKernel(new Module());
    kernel.Get<Complex>();
  }
}
