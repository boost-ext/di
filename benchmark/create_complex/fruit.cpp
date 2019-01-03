//
// Copyright (c) 2012-2019 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <memory>
#include <fruit/fruit.h>

// clang-format off
struct X00 { INJECT(X00()) { } };
struct X01 { INJECT(X01(X00)) { } };
struct X02 { INJECT(X02(X00, X01)) { } };
struct X03 { INJECT(X03(X00, X01, X02)) { } };
struct X04 { INJECT(X04(X00, X01, X02, X03)) { } };
struct X05 { INJECT(X05(X00, X01, X02, X03, X04)) { } };
struct X06 { INJECT(X06(X00, X01, X02, X03, X04, X05)) { } };
struct X07 { INJECT(X07(X00, X01, X02, X03, X04, X05, X06)) { } };
struct X08 { INJECT(X08(X00, X01, X02, X03, X04, X05, X06, X07)) { } };
struct X09 { INJECT(X09(X00, X01, X02, X03, X04, X05, X06, X07, X08)) { } };
struct X10 { INJECT(X10(X00, X01, X02, X03, X04, X05, X06, X07, X08, X09)) { } };
struct X11 { INJECT(X11(X01, X02, X03, X04, X05, X06, X07, X08, X09, X10)) { } };
struct X12 { INJECT(X12(X02, X03, X04, X05, X06, X07, X08, X09, X10, X11)) { } };
struct X13 { INJECT(X13(X03, X04, X05, X06, X07, X08, X09, X10, X11, X12)) { } };
struct X14 { INJECT(X14(X04, X05, X06, X07, X08, X09, X10, X11, X12, X13)) { } };
struct X15 { INJECT(X15(X05, X06, X07, X08, X09, X10, X11, X12, X13, X14)) { } };
struct X16 { INJECT(X16(X06, X07, X08, X09, X10, X11, X12, X13, X14, X15)) { } };
struct X17 { INJECT(X17(X07, X08, X09, X10, X11, X12, X13, X14, X15, X16)) { } };
struct X18 { INJECT(X18(X08, X09, X10, X11, X12, X13, X14, X15, X16, X17)) { } };
struct X19 { INJECT(X19(X09, X10, X11, X12, X13, X14, X15, X16, X17, X18)) { } };
struct X20 { INJECT(X20(X10, X11, X12, X13, X14, X15, X16, X17, X18, X19)) { } };
struct X21 { INJECT(X21(X11, X12, X13, X14, X15, X16, X17, X18, X19, X20)) { } };
struct X22 { INJECT(X22(X12, X13, X14, X15, X16, X17, X18, X19, X20, X21)) { } };
struct X23 { INJECT(X23(X13, X14, X15, X16, X17, X18, X19, X20, X21, X22)) { } };
struct X24 { INJECT(X24(X14, X15, X16, X17, X18, X19, X20, X21, X22, X23)) { } };
struct X25 { INJECT(X25(X15, X16, X17, X18, X19, X20, X21, X22, X23, X24)) { } };
struct X26 { INJECT(X26(X16, X17, X18, X19, X20, X21, X22, X23, X24, X25)) { } };
struct X27 { INJECT(X27(X17, X18, X19, X20, X21, X22, X23, X24, X25, X26)) { } };
struct X28 { INJECT(X28(X18, X19, X20, X21, X22, X23, X24, X25, X26, X27)) { } };
struct X29 { INJECT(X29(X19, X20, X21, X22, X23, X24, X25, X26, X27, X28)) { } };
struct X30 { INJECT(X30(X20, X21, X22, X23, X24, X25, X26, X27, X28, X29)) { } };
struct X31 { INJECT(X31(X21, X22, X23, X24, X25, X26, X27, X28, X29, X30)) { } };
struct X32 { INJECT(X32(X22, X23, X24, X25, X26, X27, X28, X29, X30, X31)) { } };
struct X33 { INJECT(X33(X23, X24, X25, X26, X27, X28, X29, X30, X31, X32)) { } };
struct X34 { INJECT(X34(X24, X25, X26, X27, X28, X29, X30, X31, X32, X33)) { } };
struct X35 { INJECT(X35(X25, X26, X27, X28, X29, X30, X31, X32, X33, X34)) { } };
struct X36 { INJECT(X36(X26, X27, X28, X29, X30, X31, X32, X33, X34, X35)) { } };
struct X37 { INJECT(X37(X27, X28, X29, X30, X31, X32, X33, X34, X35, X36)) { } };
struct X38 { INJECT(X38(X28, X29, X30, X31, X32, X33, X34, X35, X36, X37)) { } };
struct X39 { INJECT(X39(X29, X30, X31, X32, X33, X34, X35, X36, X37, X38)) { } };
struct X40 { INJECT(X40(X30, X31, X32, X33, X34, X35, X36, X37, X38, X39)) { } };
struct X41 { INJECT(X41(X31, X32, X33, X34, X35, X36, X37, X38, X39, X40)) { } };
struct X42 { INJECT(X42(X32, X33, X34, X35, X36, X37, X38, X39, X40, X41)) { } };
struct X43 { INJECT(X43(X33, X34, X35, X36, X37, X38, X39, X40, X41, X42)) { } };
struct X44 { INJECT(X44(X34, X35, X36, X37, X38, X39, X40, X41, X42, X43)) { } };
struct X45 { INJECT(X45(X35, X36, X37, X38, X39, X40, X41, X42, X43, X44)) { } };
struct X46 { INJECT(X46(X36, X37, X38, X39, X40, X41, X42, X43, X44, X45)) { } };
struct X47 { INJECT(X47(X37, X38, X39, X40, X41, X42, X43, X44, X45, X46)) { } };
struct X48 { INJECT(X48(X38, X39, X40, X41, X42, X43, X44, X45, X46, X47)) { } };
struct X49 { INJECT(X49(X39, X40, X41, X42, X43, X44, X45, X46, X47, X48)) { } };
struct X50 { INJECT(X50(X40, X41, X42, X43, X44, X45, X46, X47, X48, X49)) { } };
struct X51 { INJECT(X51(X41, X42, X43, X44, X45, X46, X47, X48, X49, X50)) { } };
struct X52 { INJECT(X52(X42, X43, X44, X45, X46, X47, X48, X49, X50, X51)) { } };
struct X53 { INJECT(X53(X43, X44, X45, X46, X47, X48, X49, X50, X51, X52)) { } };
struct X54 { INJECT(X54(X44, X45, X46, X47, X48, X49, X50, X51, X52, X53)) { } };
struct X55 { INJECT(X55(X45, X46, X47, X48, X49, X50, X51, X52, X53, X54)) { } };
struct X56 { INJECT(X56(X46, X47, X48, X49, X50, X51, X52, X53, X54, X55)) { } };
struct X57 { INJECT(X57(X47, X48, X49, X50, X51, X52, X53, X54, X55, X56)) { } };
struct X58 { INJECT(X58(X48, X49, X50, X51, X52, X53, X54, X55, X56, X57)) { } };
struct X59 { INJECT(X59(X49, X50, X51, X52, X53, X54, X55, X56, X57, X58)) { } };
struct X60 { INJECT(X60(X50, X51, X52, X53, X54, X55, X56, X57, X58, X59)) { } };
struct X61 { INJECT(X61(X51, X52, X53, X54, X55, X56, X57, X58, X59, X60)) { } };
struct X62 { INJECT(X62(X52, X53, X54, X55, X56, X57, X58, X59, X60, X61)) { } };
struct X63 { INJECT(X63(X53, X54, X55, X56, X57, X58, X59, X60, X61, X62)) { } };
struct X64 { INJECT(X64(X54, X55, X56, X57, X58, X59, X60, X61, X62, X63)) { } };
struct X65 { INJECT(X65(X55, X56, X57, X58, X59, X60, X61, X62, X63, X64)) { } };
struct X66 { INJECT(X66(X56, X57, X58, X59, X60, X61, X62, X63, X64, X65)) { } };
struct X67 { INJECT(X67(X57, X58, X59, X60, X61, X62, X63, X64, X65, X66)) { } };
struct X68 { INJECT(X68(X58, X59, X60, X61, X62, X63, X64, X65, X66, X67)) { } };
struct X69 { INJECT(X69(X59, X60, X61, X62, X63, X64, X65, X66, X67, X68)) { } };
struct X70 { INJECT(X70(X60, X61, X62, X63, X64, X65, X66, X67, X68, X69)) { } };
struct X71 { INJECT(X71(X61, X62, X63, X64, X65, X66, X67, X68, X69, X70)) { } };
struct X72 { INJECT(X72(X62, X63, X64, X65, X66, X67, X68, X69, X70, X71)) { } };
struct X73 { INJECT(X73(X63, X64, X65, X66, X67, X68, X69, X70, X71, X72)) { } };
struct X74 { INJECT(X74(X64, X65, X66, X67, X68, X69, X70, X71, X72, X73)) { } };
struct X75 { INJECT(X75(X65, X66, X67, X68, X69, X70, X71, X72, X73, X74)) { } };
struct X76 { INJECT(X76(X66, X67, X68, X69, X70, X71, X72, X73, X74, X75)) { } };
struct X77 { INJECT(X77(X67, X68, X69, X70, X71, X72, X73, X74, X75, X76)) { } };
struct X78 { INJECT(X78(X68, X69, X70, X71, X72, X73, X74, X75, X76, X77)) { } };
struct X79 { INJECT(X79(X69, X70, X71, X72, X73, X74, X75, X76, X77, X78)) { } };
struct X80 { INJECT(X80(X70, X71, X72, X73, X74, X75, X76, X77, X78, X79)) { } };
struct X81 { INJECT(X81(X71, X72, X73, X74, X75, X76, X77, X78, X79, X80)) { } };
struct X82 { INJECT(X82(X72, X73, X74, X75, X76, X77, X78, X79, X80, X81)) { } };
struct X83 { INJECT(X83(X73, X74, X75, X76, X77, X78, X79, X80, X81, X82)) { } };
struct X84 { INJECT(X84(X74, X75, X76, X77, X78, X79, X80, X81, X82, X83)) { } };
struct X85 { INJECT(X85(X75, X76, X77, X78, X79, X80, X81, X82, X83, X84)) { } };
struct X86 { INJECT(X86(X76, X77, X78, X79, X80, X81, X82, X83, X84, X85)) { } };
struct X87 { INJECT(X87(X77, X78, X79, X80, X81, X82, X83, X84, X85, X86)) { } };
struct X88 { INJECT(X88(X78, X79, X80, X81, X82, X83, X84, X85, X86, X87)) { } };
struct X89 { INJECT(X89(X79, X80, X81, X82, X83, X84, X85, X86, X87, X88)) { } };
struct X90 { INJECT(X90(X80, X81, X82, X83, X84, X85, X86, X87, X88, X89)) { } };
struct X91 { INJECT(X91(X81, X82, X83, X84, X85, X86, X87, X88, X89, X90)) { } };
struct X92 { INJECT(X92(X82, X83, X84, X85, X86, X87, X88, X89, X90, X91)) { } };
struct X93 { INJECT(X93(X83, X84, X85, X86, X87, X88, X89, X90, X91, X92)) { } };
struct X94 { INJECT(X94(X84, X85, X86, X87, X88, X89, X90, X91, X92, X93)) { } };
struct X95 { INJECT(X95(X85, X86, X87, X88, X89, X90, X91, X92, X93, X94)) { } };
struct X96 { INJECT(X96(X86, X87, X88, X89, X90, X91, X92, X93, X94, X95)) { } };
struct X97 { INJECT(X97(X87, X88, X89, X90, X91, X92, X93, X94, X95, X96)) { } };
struct X98 { INJECT(X98(X88, X89, X90, X91, X92, X93, X94, X95, X96, X97)) { } };
struct X99 { INJECT(X99(X89, X90, X91, X92, X93, X94, X95, X96, X97, X98)) { } };
struct I00 { virtual ~I00() noexcept = default; virtual void dummy() = 0; }; struct Impl00 : I00 { INJECT(Impl00(X00, X01, X02, X03, X04, X05, X06, X07, X08, X09)) { } void dummy() override { } };
struct I01 { virtual ~I01() noexcept = default; virtual void dummy() = 0; }; struct Impl01 : I01 { INJECT(Impl01(X01, X02, X03, X04, X05, X06, X07, X08, X09, X10)) { } void dummy() override { } };
struct I02 { virtual ~I02() noexcept = default; virtual void dummy() = 0; }; struct Impl02 : I02 { INJECT(Impl02(X02, X03, X04, X05, X06, X07, X08, X09, X10, X11)) { } void dummy() override { } };
struct I03 { virtual ~I03() noexcept = default; virtual void dummy() = 0; }; struct Impl03 : I03 { INJECT(Impl03(X03, X04, X05, X06, X07, X08, X09, X10, X11, X12)) { } void dummy() override { } };
struct I04 { virtual ~I04() noexcept = default; virtual void dummy() = 0; }; struct Impl04 : I04 { INJECT(Impl04(X04, X05, X06, X07, X08, X09, X10, X11, X12, X13)) { } void dummy() override { } };
struct I05 { virtual ~I05() noexcept = default; virtual void dummy() = 0; }; struct Impl05 : I05 { INJECT(Impl05(X05, X06, X07, X08, X09, X10, X11, X12, X13, X14)) { } void dummy() override { } };
struct I06 { virtual ~I06() noexcept = default; virtual void dummy() = 0; }; struct Impl06 : I06 { INJECT(Impl06(X06, X07, X08, X09, X10, X11, X12, X13, X14, X15)) { } void dummy() override { } };
struct I07 { virtual ~I07() noexcept = default; virtual void dummy() = 0; }; struct Impl07 : I07 { INJECT(Impl07(X07, X08, X09, X10, X11, X12, X13, X14, X15, X16)) { } void dummy() override { } };
struct I08 { virtual ~I08() noexcept = default; virtual void dummy() = 0; }; struct Impl08 : I08 { INJECT(Impl08(X08, X09, X10, X11, X12, X13, X14, X15, X16, X17)) { } void dummy() override { } };
struct I09 { virtual ~I09() noexcept = default; virtual void dummy() = 0; }; struct Impl09 : I09 { INJECT(Impl09(X09, X10, X11, X12, X13, X14, X15, X16, X17, X18)) { } void dummy() override { } };
struct I10 { virtual ~I10() noexcept = default; virtual void dummy() = 0; }; struct Impl10 : I10 { INJECT(Impl10(X10, X11, X12, X13, X14, X15, X16, X17, X18, X19)) { } void dummy() override { } };
struct I11 { virtual ~I11() noexcept = default; virtual void dummy() = 0; }; struct Impl11 : I11 { INJECT(Impl11(X11, X12, X13, X14, X15, X16, X17, X18, X19, X20)) { } void dummy() override { } };
struct I12 { virtual ~I12() noexcept = default; virtual void dummy() = 0; }; struct Impl12 : I12 { INJECT(Impl12(X12, X13, X14, X15, X16, X17, X18, X19, X20, X21)) { } void dummy() override { } };
struct I13 { virtual ~I13() noexcept = default; virtual void dummy() = 0; }; struct Impl13 : I13 { INJECT(Impl13(X13, X14, X15, X16, X17, X18, X19, X20, X21, X22)) { } void dummy() override { } };
struct I14 { virtual ~I14() noexcept = default; virtual void dummy() = 0; }; struct Impl14 : I14 { INJECT(Impl14(X14, X15, X16, X17, X18, X19, X20, X21, X22, X23)) { } void dummy() override { } };
struct I15 { virtual ~I15() noexcept = default; virtual void dummy() = 0; }; struct Impl15 : I15 { INJECT(Impl15(X15, X16, X17, X18, X19, X20, X21, X22, X23, X24)) { } void dummy() override { } };
struct I16 { virtual ~I16() noexcept = default; virtual void dummy() = 0; }; struct Impl16 : I16 { INJECT(Impl16(X16, X17, X18, X19, X20, X21, X22, X23, X24, X25)) { } void dummy() override { } };
struct I17 { virtual ~I17() noexcept = default; virtual void dummy() = 0; }; struct Impl17 : I17 { INJECT(Impl17(X17, X18, X19, X20, X21, X22, X23, X24, X25, X26)) { } void dummy() override { } };
struct I18 { virtual ~I18() noexcept = default; virtual void dummy() = 0; }; struct Impl18 : I18 { INJECT(Impl18(X18, X19, X20, X21, X22, X23, X24, X25, X26, X27)) { } void dummy() override { } };
struct I19 { virtual ~I19() noexcept = default; virtual void dummy() = 0; }; struct Impl19 : I19 { INJECT(Impl19(X19, X20, X21, X22, X23, X24, X25, X26, X27, X28)) { } void dummy() override { } };
struct I20 { virtual ~I20() noexcept = default; virtual void dummy() = 0; }; struct Impl20 : I20 { INJECT(Impl20(X20, X21, X22, X23, X24, X25, X26, X27, X28, X29)) { } void dummy() override { } };
struct I21 { virtual ~I21() noexcept = default; virtual void dummy() = 0; }; struct Impl21 : I21 { INJECT(Impl21(X21, X22, X23, X24, X25, X26, X27, X28, X29, X30)) { } void dummy() override { } };
struct I22 { virtual ~I22() noexcept = default; virtual void dummy() = 0; }; struct Impl22 : I22 { INJECT(Impl22(X22, X23, X24, X25, X26, X27, X28, X29, X30, X31)) { } void dummy() override { } };
struct I23 { virtual ~I23() noexcept = default; virtual void dummy() = 0; }; struct Impl23 : I23 { INJECT(Impl23(X23, X24, X25, X26, X27, X28, X29, X30, X31, X32)) { } void dummy() override { } };
struct I24 { virtual ~I24() noexcept = default; virtual void dummy() = 0; }; struct Impl24 : I24 { INJECT(Impl24(X24, X25, X26, X27, X28, X29, X30, X31, X32, X33)) { } void dummy() override { } };
struct I25 { virtual ~I25() noexcept = default; virtual void dummy() = 0; }; struct Impl25 : I25 { INJECT(Impl25(X25, X26, X27, X28, X29, X30, X31, X32, X33, X34)) { } void dummy() override { } };
struct I26 { virtual ~I26() noexcept = default; virtual void dummy() = 0; }; struct Impl26 : I26 { INJECT(Impl26(X26, X27, X28, X29, X30, X31, X32, X33, X34, X35)) { } void dummy() override { } };
struct I27 { virtual ~I27() noexcept = default; virtual void dummy() = 0; }; struct Impl27 : I27 { INJECT(Impl27(X27, X28, X29, X30, X31, X32, X33, X34, X35, X36)) { } void dummy() override { } };
struct I28 { virtual ~I28() noexcept = default; virtual void dummy() = 0; }; struct Impl28 : I28 { INJECT(Impl28(X28, X29, X30, X31, X32, X33, X34, X35, X36, X37)) { } void dummy() override { } };
struct I29 { virtual ~I29() noexcept = default; virtual void dummy() = 0; }; struct Impl29 : I29 { INJECT(Impl29(X29, X30, X31, X32, X33, X34, X35, X36, X37, X38)) { } void dummy() override { } };
struct I30 { virtual ~I30() noexcept = default; virtual void dummy() = 0; }; struct Impl30 : I30 { INJECT(Impl30(X30, X31, X32, X33, X34, X35, X36, X37, X38, X39)) { } void dummy() override { } };
struct I31 { virtual ~I31() noexcept = default; virtual void dummy() = 0; }; struct Impl31 : I31 { INJECT(Impl31(X31, X32, X33, X34, X35, X36, X37, X38, X39, X40)) { } void dummy() override { } };
struct I32 { virtual ~I32() noexcept = default; virtual void dummy() = 0; }; struct Impl32 : I32 { INJECT(Impl32(X32, X33, X34, X35, X36, X37, X38, X39, X40, X41)) { } void dummy() override { } };
struct I33 { virtual ~I33() noexcept = default; virtual void dummy() = 0; }; struct Impl33 : I33 { INJECT(Impl33(X33, X34, X35, X36, X37, X38, X39, X40, X41, X42)) { } void dummy() override { } };
struct I34 { virtual ~I34() noexcept = default; virtual void dummy() = 0; }; struct Impl34 : I34 { INJECT(Impl34(X34, X35, X36, X37, X38, X39, X40, X41, X42, X43)) { } void dummy() override { } };
struct I35 { virtual ~I35() noexcept = default; virtual void dummy() = 0; }; struct Impl35 : I35 { INJECT(Impl35(X35, X36, X37, X38, X39, X40, X41, X42, X43, X44)) { } void dummy() override { } };
struct I36 { virtual ~I36() noexcept = default; virtual void dummy() = 0; }; struct Impl36 : I36 { INJECT(Impl36(X36, X37, X38, X39, X40, X41, X42, X43, X44, X45)) { } void dummy() override { } };
struct I37 { virtual ~I37() noexcept = default; virtual void dummy() = 0; }; struct Impl37 : I37 { INJECT(Impl37(X37, X38, X39, X40, X41, X42, X43, X44, X45, X46)) { } void dummy() override { } };
struct I38 { virtual ~I38() noexcept = default; virtual void dummy() = 0; }; struct Impl38 : I38 { INJECT(Impl38(X38, X39, X40, X41, X42, X43, X44, X45, X46, X47)) { } void dummy() override { } };
struct I39 { virtual ~I39() noexcept = default; virtual void dummy() = 0; }; struct Impl39 : I39 { INJECT(Impl39(X39, X40, X41, X42, X43, X44, X45, X46, X47, X48)) { } void dummy() override { } };
struct I40 { virtual ~I40() noexcept = default; virtual void dummy() = 0; }; struct Impl40 : I40 { INJECT(Impl40(X40, X41, X42, X43, X44, X45, X46, X47, X48, X49)) { } void dummy() override { } };
struct I41 { virtual ~I41() noexcept = default; virtual void dummy() = 0; }; struct Impl41 : I41 { INJECT(Impl41(X41, X42, X43, X44, X45, X46, X47, X48, X49, X50)) { } void dummy() override { } };
struct I42 { virtual ~I42() noexcept = default; virtual void dummy() = 0; }; struct Impl42 : I42 { INJECT(Impl42(X42, X43, X44, X45, X46, X47, X48, X49, X50, X51)) { } void dummy() override { } };
struct I43 { virtual ~I43() noexcept = default; virtual void dummy() = 0; }; struct Impl43 : I43 { INJECT(Impl43(X43, X44, X45, X46, X47, X48, X49, X50, X51, X52)) { } void dummy() override { } };
struct I44 { virtual ~I44() noexcept = default; virtual void dummy() = 0; }; struct Impl44 : I44 { INJECT(Impl44(X44, X45, X46, X47, X48, X49, X50, X51, X52, X53)) { } void dummy() override { } };
struct I45 { virtual ~I45() noexcept = default; virtual void dummy() = 0; }; struct Impl45 : I45 { INJECT(Impl45(X45, X46, X47, X48, X49, X50, X51, X52, X53, X54)) { } void dummy() override { } };
struct I46 { virtual ~I46() noexcept = default; virtual void dummy() = 0; }; struct Impl46 : I46 { INJECT(Impl46(X46, X47, X48, X49, X50, X51, X52, X53, X54, X55)) { } void dummy() override { } };
struct I47 { virtual ~I47() noexcept = default; virtual void dummy() = 0; }; struct Impl47 : I47 { INJECT(Impl47(X47, X48, X49, X50, X51, X52, X53, X54, X55, X56)) { } void dummy() override { } };
struct I48 { virtual ~I48() noexcept = default; virtual void dummy() = 0; }; struct Impl48 : I48 { INJECT(Impl48(X48, X49, X50, X51, X52, X53, X54, X55, X56, X57)) { } void dummy() override { } };
struct I49 { virtual ~I49() noexcept = default; virtual void dummy() = 0; }; struct Impl49 : I49 { INJECT(Impl49(X49, X50, X51, X52, X53, X54, X55, X56, X57, X58)) { } void dummy() override { } };
struct I50 { virtual ~I50() noexcept = default; virtual void dummy() = 0; }; struct Impl50 : I50 { INJECT(Impl50(X50, X51, X52, X53, X54, X55, X56, X57, X58, X59)) { } void dummy() override { } };
struct I51 { virtual ~I51() noexcept = default; virtual void dummy() = 0; }; struct Impl51 : I51 { INJECT(Impl51(X51, X52, X53, X54, X55, X56, X57, X58, X59, X60)) { } void dummy() override { } };
struct I52 { virtual ~I52() noexcept = default; virtual void dummy() = 0; }; struct Impl52 : I52 { INJECT(Impl52(X52, X53, X54, X55, X56, X57, X58, X59, X60, X61)) { } void dummy() override { } };
struct I53 { virtual ~I53() noexcept = default; virtual void dummy() = 0; }; struct Impl53 : I53 { INJECT(Impl53(X53, X54, X55, X56, X57, X58, X59, X60, X61, X62)) { } void dummy() override { } };
struct I54 { virtual ~I54() noexcept = default; virtual void dummy() = 0; }; struct Impl54 : I54 { INJECT(Impl54(X54, X55, X56, X57, X58, X59, X60, X61, X62, X63)) { } void dummy() override { } };
struct I55 { virtual ~I55() noexcept = default; virtual void dummy() = 0; }; struct Impl55 : I55 { INJECT(Impl55(X55, X56, X57, X58, X59, X60, X61, X62, X63, X64)) { } void dummy() override { } };
struct I56 { virtual ~I56() noexcept = default; virtual void dummy() = 0; }; struct Impl56 : I56 { INJECT(Impl56(X56, X57, X58, X59, X60, X61, X62, X63, X64, X65)) { } void dummy() override { } };
struct I57 { virtual ~I57() noexcept = default; virtual void dummy() = 0; }; struct Impl57 : I57 { INJECT(Impl57(X57, X58, X59, X60, X61, X62, X63, X64, X65, X66)) { } void dummy() override { } };
struct I58 { virtual ~I58() noexcept = default; virtual void dummy() = 0; }; struct Impl58 : I58 { INJECT(Impl58(X58, X59, X60, X61, X62, X63, X64, X65, X66, X67)) { } void dummy() override { } };
struct I59 { virtual ~I59() noexcept = default; virtual void dummy() = 0; }; struct Impl59 : I59 { INJECT(Impl59(X59, X60, X61, X62, X63, X64, X65, X66, X67, X68)) { } void dummy() override { } };
struct I60 { virtual ~I60() noexcept = default; virtual void dummy() = 0; }; struct Impl60 : I60 { INJECT(Impl60(X60, X61, X62, X63, X64, X65, X66, X67, X68, X69)) { } void dummy() override { } };
struct I61 { virtual ~I61() noexcept = default; virtual void dummy() = 0; }; struct Impl61 : I61 { INJECT(Impl61(X61, X62, X63, X64, X65, X66, X67, X68, X69, X70)) { } void dummy() override { } };
struct I62 { virtual ~I62() noexcept = default; virtual void dummy() = 0; }; struct Impl62 : I62 { INJECT(Impl62(X62, X63, X64, X65, X66, X67, X68, X69, X70, X71)) { } void dummy() override { } };
struct I63 { virtual ~I63() noexcept = default; virtual void dummy() = 0; }; struct Impl63 : I63 { INJECT(Impl63(X63, X64, X65, X66, X67, X68, X69, X70, X71, X72)) { } void dummy() override { } };
struct I64 { virtual ~I64() noexcept = default; virtual void dummy() = 0; }; struct Impl64 : I64 { INJECT(Impl64(X64, X65, X66, X67, X68, X69, X70, X71, X72, X73)) { } void dummy() override { } };
struct I65 { virtual ~I65() noexcept = default; virtual void dummy() = 0; }; struct Impl65 : I65 { INJECT(Impl65(X65, X66, X67, X68, X69, X70, X71, X72, X73, X74)) { } void dummy() override { } };
struct I66 { virtual ~I66() noexcept = default; virtual void dummy() = 0; }; struct Impl66 : I66 { INJECT(Impl66(X66, X67, X68, X69, X70, X71, X72, X73, X74, X75)) { } void dummy() override { } };
struct I67 { virtual ~I67() noexcept = default; virtual void dummy() = 0; }; struct Impl67 : I67 { INJECT(Impl67(X67, X68, X69, X70, X71, X72, X73, X74, X75, X76)) { } void dummy() override { } };
struct I68 { virtual ~I68() noexcept = default; virtual void dummy() = 0; }; struct Impl68 : I68 { INJECT(Impl68(X68, X69, X70, X71, X72, X73, X74, X75, X76, X77)) { } void dummy() override { } };
struct I69 { virtual ~I69() noexcept = default; virtual void dummy() = 0; }; struct Impl69 : I69 { INJECT(Impl69(X69, X70, X71, X72, X73, X74, X75, X76, X77, X78)) { } void dummy() override { } };
struct I70 { virtual ~I70() noexcept = default; virtual void dummy() = 0; }; struct Impl70 : I70 { INJECT(Impl70(X70, X71, X72, X73, X74, X75, X76, X77, X78, X79)) { } void dummy() override { } };
struct I71 { virtual ~I71() noexcept = default; virtual void dummy() = 0; }; struct Impl71 : I71 { INJECT(Impl71(X71, X72, X73, X74, X75, X76, X77, X78, X79, X80)) { } void dummy() override { } };
struct I72 { virtual ~I72() noexcept = default; virtual void dummy() = 0; }; struct Impl72 : I72 { INJECT(Impl72(X72, X73, X74, X75, X76, X77, X78, X79, X80, X81)) { } void dummy() override { } };
struct I73 { virtual ~I73() noexcept = default; virtual void dummy() = 0; }; struct Impl73 : I73 { INJECT(Impl73(X73, X74, X75, X76, X77, X78, X79, X80, X81, X82)) { } void dummy() override { } };
struct I74 { virtual ~I74() noexcept = default; virtual void dummy() = 0; }; struct Impl74 : I74 { INJECT(Impl74(X74, X75, X76, X77, X78, X79, X80, X81, X82, X83)) { } void dummy() override { } };
struct I75 { virtual ~I75() noexcept = default; virtual void dummy() = 0; }; struct Impl75 : I75 { INJECT(Impl75(X75, X76, X77, X78, X79, X80, X81, X82, X83, X84)) { } void dummy() override { } };
struct I76 { virtual ~I76() noexcept = default; virtual void dummy() = 0; }; struct Impl76 : I76 { INJECT(Impl76(X76, X77, X78, X79, X80, X81, X82, X83, X84, X85)) { } void dummy() override { } };
struct I77 { virtual ~I77() noexcept = default; virtual void dummy() = 0; }; struct Impl77 : I77 { INJECT(Impl77(X77, X78, X79, X80, X81, X82, X83, X84, X85, X86)) { } void dummy() override { } };
struct I78 { virtual ~I78() noexcept = default; virtual void dummy() = 0; }; struct Impl78 : I78 { INJECT(Impl78(X78, X79, X80, X81, X82, X83, X84, X85, X86, X87)) { } void dummy() override { } };
struct I79 { virtual ~I79() noexcept = default; virtual void dummy() = 0; }; struct Impl79 : I79 { INJECT(Impl79(X79, X80, X81, X82, X83, X84, X85, X86, X87, X88)) { } void dummy() override { } };
struct I80 { virtual ~I80() noexcept = default; virtual void dummy() = 0; }; struct Impl80 : I80 { INJECT(Impl80(X80, X81, X82, X83, X84, X85, X86, X87, X88, X89)) { } void dummy() override { } };
struct I81 { virtual ~I81() noexcept = default; virtual void dummy() = 0; }; struct Impl81 : I81 { INJECT(Impl81(X81, X82, X83, X84, X85, X86, X87, X88, X89, X90)) { } void dummy() override { } };
struct I82 { virtual ~I82() noexcept = default; virtual void dummy() = 0; }; struct Impl82 : I82 { INJECT(Impl82(X82, X83, X84, X85, X86, X87, X88, X89, X90, X91)) { } void dummy() override { } };
struct I83 { virtual ~I83() noexcept = default; virtual void dummy() = 0; }; struct Impl83 : I83 { INJECT(Impl83(X83, X84, X85, X86, X87, X88, X89, X90, X91, X92)) { } void dummy() override { } };
struct I84 { virtual ~I84() noexcept = default; virtual void dummy() = 0; }; struct Impl84 : I84 { INJECT(Impl84(X84, X85, X86, X87, X88, X89, X90, X91, X92, X93)) { } void dummy() override { } };
struct I85 { virtual ~I85() noexcept = default; virtual void dummy() = 0; }; struct Impl85 : I85 { INJECT(Impl85(X85, X86, X87, X88, X89, X90, X91, X92, X93, X94)) { } void dummy() override { } };
struct I86 { virtual ~I86() noexcept = default; virtual void dummy() = 0; }; struct Impl86 : I86 { INJECT(Impl86(X86, X87, X88, X89, X90, X91, X92, X93, X94, X95)) { } void dummy() override { } };
struct I87 { virtual ~I87() noexcept = default; virtual void dummy() = 0; }; struct Impl87 : I87 { INJECT(Impl87(X87, X88, X89, X90, X91, X92, X93, X94, X95, X96)) { } void dummy() override { } };
struct I88 { virtual ~I88() noexcept = default; virtual void dummy() = 0; }; struct Impl88 : I88 { INJECT(Impl88(X88, X89, X90, X91, X92, X93, X94, X95, X96, X97)) { } void dummy() override { } };
struct I89 { virtual ~I89() noexcept = default; virtual void dummy() = 0; }; struct Impl89 : I89 { INJECT(Impl89(X89, X90, X91, X92, X93, X94, X95, X96, X97, X98)) { } void dummy() override { } };
struct I90 { virtual ~I90() noexcept = default; virtual void dummy() = 0; }; struct Impl90 : I90 { INJECT(Impl90(X90, X91, X92, X93, X94, X95, X96, X97, X98, X99)) { } void dummy() override { } };
struct I91 { virtual ~I91() noexcept = default; virtual void dummy() = 0; }; struct Impl91 : I91 { INJECT(Impl91(X91, X92, X93, X94, X95, X96, X97, X98, X99, X00)) { } void dummy() override { } };
struct I92 { virtual ~I92() noexcept = default; virtual void dummy() = 0; }; struct Impl92 : I92 { INJECT(Impl92(X92, X93, X94, X95, X96, X97, X98, X99, X00, X01)) { } void dummy() override { } };
struct I93 { virtual ~I93() noexcept = default; virtual void dummy() = 0; }; struct Impl93 : I93 { INJECT(Impl93(X93, X94, X95, X96, X97, X98, X99, X00, X01, X02)) { } void dummy() override { } };
struct I94 { virtual ~I94() noexcept = default; virtual void dummy() = 0; }; struct Impl94 : I94 { INJECT(Impl94(X94, X95, X96, X97, X98, X99, X00, X01, X02, X03)) { } void dummy() override { } };
struct I95 { virtual ~I95() noexcept = default; virtual void dummy() = 0; }; struct Impl95 : I95 { INJECT(Impl95(X95, X96, X97, X98, X99, X00, X01, X02, X03, X04)) { } void dummy() override { } };
struct I96 { virtual ~I96() noexcept = default; virtual void dummy() = 0; }; struct Impl96 : I96 { INJECT(Impl96(X96, X97, X98, X99, X00, X01, X02, X03, X04, X05)) { } void dummy() override { } };
struct I97 { virtual ~I97() noexcept = default; virtual void dummy() = 0; }; struct Impl97 : I97 { INJECT(Impl97(X97, X98, X99, X00, X01, X02, X03, X04, X05, X06)) { } void dummy() override { } };
struct I98 { virtual ~I98() noexcept = default; virtual void dummy() = 0; }; struct Impl98 : I98 { INJECT(Impl98(X98, X99, X00, X01, X02, X03, X04, X05, X06, X07)) { } void dummy() override { } };
struct I99 { virtual ~I99() noexcept = default; virtual void dummy() = 0; }; struct Impl99 : I99 { INJECT(Impl99(X99, X00, X01, X02, X03, X04, X05, X06, X07, X08)) { } void dummy() override { } };
struct C0 { INJECT(C0(std::shared_ptr<I00>, std::shared_ptr<I01>, std::shared_ptr<I02>, std::shared_ptr<I03>, std::shared_ptr<I04>, std::shared_ptr<I05>, std::shared_ptr<I06>, std::shared_ptr<I07>, std::shared_ptr<I08>, std::shared_ptr<I09>)) { } };
struct C1 { INJECT(C1(std::shared_ptr<I10>, std::shared_ptr<I11>, std::shared_ptr<I12>, std::shared_ptr<I13>, std::shared_ptr<I14>, std::shared_ptr<I15>, std::shared_ptr<I16>, std::shared_ptr<I17>, std::shared_ptr<I18>, std::shared_ptr<I19>)) { } };
struct C2 { INJECT(C2(std::shared_ptr<I20>, std::shared_ptr<I21>, std::shared_ptr<I22>, std::shared_ptr<I23>, std::shared_ptr<I24>, std::shared_ptr<I25>, std::shared_ptr<I26>, std::shared_ptr<I27>, std::shared_ptr<I28>, std::shared_ptr<I29>)) { } };
struct C3 { INJECT(C3(std::shared_ptr<I30>, std::shared_ptr<I31>, std::shared_ptr<I32>, std::shared_ptr<I33>, std::shared_ptr<I34>, std::shared_ptr<I35>, std::shared_ptr<I36>, std::shared_ptr<I37>, std::shared_ptr<I38>, std::shared_ptr<I39>)) { } };
struct C4 { INJECT(C4(std::shared_ptr<I40>, std::shared_ptr<I41>, std::shared_ptr<I42>, std::shared_ptr<I43>, std::shared_ptr<I44>, std::shared_ptr<I45>, std::shared_ptr<I46>, std::shared_ptr<I47>, std::shared_ptr<I48>, std::shared_ptr<I49>)) { } };
struct C5 { INJECT(C5(std::shared_ptr<I50>, std::shared_ptr<I51>, std::shared_ptr<I52>, std::shared_ptr<I53>, std::shared_ptr<I54>, std::shared_ptr<I55>, std::shared_ptr<I56>, std::shared_ptr<I57>, std::shared_ptr<I58>, std::shared_ptr<I59>)) { } };
struct C6 { INJECT(C6(std::shared_ptr<I60>, std::shared_ptr<I61>, std::shared_ptr<I62>, std::shared_ptr<I63>, std::shared_ptr<I64>, std::shared_ptr<I65>, std::shared_ptr<I66>, std::shared_ptr<I67>, std::shared_ptr<I68>, std::shared_ptr<I69>)) { } };
struct C7 { INJECT(C7(std::shared_ptr<I70>, std::shared_ptr<I71>, std::shared_ptr<I72>, std::shared_ptr<I73>, std::shared_ptr<I74>, std::shared_ptr<I75>, std::shared_ptr<I76>, std::shared_ptr<I77>, std::shared_ptr<I78>, std::shared_ptr<I79>)) { } };
struct C8 { INJECT(C8(std::shared_ptr<I80>, std::shared_ptr<I81>, std::shared_ptr<I82>, std::shared_ptr<I83>, std::shared_ptr<I84>, std::shared_ptr<I85>, std::shared_ptr<I86>, std::shared_ptr<I87>, std::shared_ptr<I88>, std::shared_ptr<I89>)) { } };
struct C9 { INJECT(C9(std::shared_ptr<I90>, std::shared_ptr<I91>, std::shared_ptr<I92>, std::shared_ptr<I93>, std::shared_ptr<I94>, std::shared_ptr<I95>, std::shared_ptr<I96>, std::shared_ptr<I97>, std::shared_ptr<I98>, std::shared_ptr<I99>)) { } };
struct Complex { INJECT(Complex(C0, C1, C2, C3, C4, C5, C6, C7, C8, C9)) { } };
// clang-format off

fruit::Component<Complex> module() {
  return fruit::createComponent()
    .bind<I00, Impl00>()
    .bind<I01, Impl01>()
    .bind<I02, Impl02>()
    .bind<I03, Impl03>()
    .bind<I04, Impl04>()
    .bind<I05, Impl05>()
    .bind<I06, Impl06>()
    .bind<I07, Impl07>()
    .bind<I08, Impl08>()
    .bind<I09, Impl09>()
    .bind<I10, Impl10>()
    .bind<I11, Impl11>()
    .bind<I12, Impl12>()
    .bind<I13, Impl13>()
    .bind<I14, Impl14>()
    .bind<I15, Impl15>()
    .bind<I16, Impl16>()
    .bind<I17, Impl17>()
    .bind<I18, Impl18>()
    .bind<I19, Impl19>()
    .bind<I20, Impl20>()
    .bind<I21, Impl21>()
    .bind<I22, Impl22>()
    .bind<I23, Impl23>()
    .bind<I24, Impl24>()
    .bind<I25, Impl25>()
    .bind<I26, Impl26>()
    .bind<I27, Impl27>()
    .bind<I28, Impl28>()
    .bind<I29, Impl29>()
    .bind<I30, Impl30>()
    .bind<I31, Impl31>()
    .bind<I32, Impl32>()
    .bind<I33, Impl33>()
    .bind<I34, Impl34>()
    .bind<I35, Impl35>()
    .bind<I36, Impl36>()
    .bind<I37, Impl37>()
    .bind<I38, Impl38>()
    .bind<I39, Impl39>()
    .bind<I40, Impl40>()
    .bind<I41, Impl41>()
    .bind<I42, Impl42>()
    .bind<I43, Impl43>()
    .bind<I44, Impl44>()
    .bind<I45, Impl45>()
    .bind<I46, Impl46>()
    .bind<I47, Impl47>()
    .bind<I48, Impl48>()
    .bind<I49, Impl49>()
    .bind<I50, Impl50>()
    .bind<I51, Impl51>()
    .bind<I52, Impl52>()
    .bind<I53, Impl53>()
    .bind<I54, Impl54>()
    .bind<I55, Impl55>()
    .bind<I56, Impl56>()
    .bind<I57, Impl57>()
    .bind<I58, Impl58>()
    .bind<I59, Impl59>()
    .bind<I60, Impl60>()
    .bind<I61, Impl61>()
    .bind<I62, Impl62>()
    .bind<I63, Impl63>()
    .bind<I64, Impl64>()
    .bind<I65, Impl65>()
    .bind<I66, Impl66>()
    .bind<I67, Impl67>()
    .bind<I68, Impl68>()
    .bind<I69, Impl69>()
    .bind<I70, Impl70>()
    .bind<I71, Impl71>()
    .bind<I72, Impl72>()
    .bind<I73, Impl73>()
    .bind<I74, Impl74>()
    .bind<I75, Impl75>()
    .bind<I76, Impl76>()
    .bind<I77, Impl77>()
    .bind<I78, Impl78>()
    .bind<I79, Impl79>()
    .bind<I80, Impl80>()
    .bind<I81, Impl81>()
    .bind<I82, Impl82>()
    .bind<I83, Impl83>()
    .bind<I84, Impl84>()
    .bind<I85, Impl85>()
    .bind<I86, Impl86>()
    .bind<I87, Impl87>()
    .bind<I88, Impl88>()
    .bind<I89, Impl89>()
    .bind<I90, Impl90>()
    .bind<I91, Impl91>()
    .bind<I92, Impl92>()
    .bind<I93, Impl93>()
    .bind<I94, Impl94>()
    .bind<I95, Impl95>()
    .bind<I96, Impl96>()
    .bind<I97, Impl97>()
    .bind<I98, Impl98>()
    .bind<I99, Impl99>();
}

int main() {
  fruit::Injector<Complex> injector{module()};
  injector.get<Complex>();
}
