//
// Copyright (c) 2012-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
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
struct Simple { INJECT(Simple(X99, X89, X79, X69, X59, X49, X39, X29, X19, X09)) { } };
// clang-format on

fruit::Component<Simple> module() { return fruit::createComponent(); }

int main() {
  fruit::Injector<Simple> injector{module()};
  injector.get<Simple>();
}
