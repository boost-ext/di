//
// Copyright (c) 2012-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <memory>
#include <boost/di.hpp>

namespace di = boost::di;

// clang-format off
struct X00 { BOOST_DI_INJECT(X00) { } };
struct X01 { BOOST_DI_INJECT(X01, X00) { } };
struct X02 { BOOST_DI_INJECT(X02, X00, X01) { } };
struct X03 { BOOST_DI_INJECT(X03, X00, X01, X02) { } };
struct X04 { BOOST_DI_INJECT(X04, X00, X01, X02, X03) { } };
struct X05 { BOOST_DI_INJECT(X05, X00, X01, X02, X03, X04) { } };
struct X06 { BOOST_DI_INJECT(X06, X00, X01, X02, X03, X04, X05) { } };
struct X07 { BOOST_DI_INJECT(X07, X00, X01, X02, X03, X04, X05, X06) { } };
struct X08 { BOOST_DI_INJECT(X08, X00, X01, X02, X03, X04, X05, X06, X07) { } };
struct X09 { BOOST_DI_INJECT(X09, X00, X01, X02, X03, X04, X05, X06, X07, X08) { } };
struct X10 { BOOST_DI_INJECT(X10, X00, X01, X02, X03, X04, X05, X06, X07, X08, X09) { } };
struct X11 { BOOST_DI_INJECT(X11, X01, X02, X03, X04, X05, X06, X07, X08, X09, X10) { } };
struct X12 { BOOST_DI_INJECT(X12, X02, X03, X04, X05, X06, X07, X08, X09, X10, X11) { } };
struct X13 { BOOST_DI_INJECT(X13, X03, X04, X05, X06, X07, X08, X09, X10, X11, X12) { } };
struct X14 { BOOST_DI_INJECT(X14, X04, X05, X06, X07, X08, X09, X10, X11, X12, X13) { } };
struct X15 { BOOST_DI_INJECT(X15, X05, X06, X07, X08, X09, X10, X11, X12, X13, X14) { } };
struct X16 { BOOST_DI_INJECT(X16, X06, X07, X08, X09, X10, X11, X12, X13, X14, X15) { } };
struct X17 { BOOST_DI_INJECT(X17, X07, X08, X09, X10, X11, X12, X13, X14, X15, X16) { } };
struct X18 { BOOST_DI_INJECT(X18, X08, X09, X10, X11, X12, X13, X14, X15, X16, X17) { } };
struct X19 { BOOST_DI_INJECT(X19, X09, X10, X11, X12, X13, X14, X15, X16, X17, X18) { } };
struct X20 { BOOST_DI_INJECT(X20, X10, X11, X12, X13, X14, X15, X16, X17, X18, X19) { } };
struct X21 { BOOST_DI_INJECT(X21, X11, X12, X13, X14, X15, X16, X17, X18, X19, X20) { } };
struct X22 { BOOST_DI_INJECT(X22, X12, X13, X14, X15, X16, X17, X18, X19, X20, X21) { } };
struct X23 { BOOST_DI_INJECT(X23, X13, X14, X15, X16, X17, X18, X19, X20, X21, X22) { } };
struct X24 { BOOST_DI_INJECT(X24, X14, X15, X16, X17, X18, X19, X20, X21, X22, X23) { } };
struct X25 { BOOST_DI_INJECT(X25, X15, X16, X17, X18, X19, X20, X21, X22, X23, X24) { } };
struct X26 { BOOST_DI_INJECT(X26, X16, X17, X18, X19, X20, X21, X22, X23, X24, X25) { } };
struct X27 { BOOST_DI_INJECT(X27, X17, X18, X19, X20, X21, X22, X23, X24, X25, X26) { } };
struct X28 { BOOST_DI_INJECT(X28, X18, X19, X20, X21, X22, X23, X24, X25, X26, X27) { } };
struct X29 { BOOST_DI_INJECT(X29, X19, X20, X21, X22, X23, X24, X25, X26, X27, X28) { } };
struct X30 { BOOST_DI_INJECT(X30, X20, X21, X22, X23, X24, X25, X26, X27, X28, X29) { } };
struct X31 { BOOST_DI_INJECT(X31, X21, X22, X23, X24, X25, X26, X27, X28, X29, X30) { } };
struct X32 { BOOST_DI_INJECT(X32, X22, X23, X24, X25, X26, X27, X28, X29, X30, X31) { } };
struct X33 { BOOST_DI_INJECT(X33, X23, X24, X25, X26, X27, X28, X29, X30, X31, X32) { } };
struct X34 { BOOST_DI_INJECT(X34, X24, X25, X26, X27, X28, X29, X30, X31, X32, X33) { } };
struct X35 { BOOST_DI_INJECT(X35, X25, X26, X27, X28, X29, X30, X31, X32, X33, X34) { } };
struct X36 { BOOST_DI_INJECT(X36, X26, X27, X28, X29, X30, X31, X32, X33, X34, X35) { } };
struct X37 { BOOST_DI_INJECT(X37, X27, X28, X29, X30, X31, X32, X33, X34, X35, X36) { } };
struct X38 { BOOST_DI_INJECT(X38, X28, X29, X30, X31, X32, X33, X34, X35, X36, X37) { } };
struct X39 { BOOST_DI_INJECT(X39, X29, X30, X31, X32, X33, X34, X35, X36, X37, X38) { } };
struct X40 { BOOST_DI_INJECT(X40, X30, X31, X32, X33, X34, X35, X36, X37, X38, X39) { } };
struct X41 { BOOST_DI_INJECT(X41, X31, X32, X33, X34, X35, X36, X37, X38, X39, X40) { } };
struct X42 { BOOST_DI_INJECT(X42, X32, X33, X34, X35, X36, X37, X38, X39, X40, X41) { } };
struct X43 { BOOST_DI_INJECT(X43, X33, X34, X35, X36, X37, X38, X39, X40, X41, X42) { } };
struct X44 { BOOST_DI_INJECT(X44, X34, X35, X36, X37, X38, X39, X40, X41, X42, X43) { } };
struct X45 { BOOST_DI_INJECT(X45, X35, X36, X37, X38, X39, X40, X41, X42, X43, X44) { } };
struct X46 { BOOST_DI_INJECT(X46, X36, X37, X38, X39, X40, X41, X42, X43, X44, X45) { } };
struct X47 { BOOST_DI_INJECT(X47, X37, X38, X39, X40, X41, X42, X43, X44, X45, X46) { } };
struct X48 { BOOST_DI_INJECT(X48, X38, X39, X40, X41, X42, X43, X44, X45, X46, X47) { } };
struct X49 { BOOST_DI_INJECT(X49, X39, X40, X41, X42, X43, X44, X45, X46, X47, X48) { } };
struct X50 { BOOST_DI_INJECT(X50, X40, X41, X42, X43, X44, X45, X46, X47, X48, X49) { } };
struct X51 { BOOST_DI_INJECT(X51, X41, X42, X43, X44, X45, X46, X47, X48, X49, X50) { } };
struct X52 { BOOST_DI_INJECT(X52, X42, X43, X44, X45, X46, X47, X48, X49, X50, X51) { } };
struct X53 { BOOST_DI_INJECT(X53, X43, X44, X45, X46, X47, X48, X49, X50, X51, X52) { } };
struct X54 { BOOST_DI_INJECT(X54, X44, X45, X46, X47, X48, X49, X50, X51, X52, X53) { } };
struct X55 { BOOST_DI_INJECT(X55, X45, X46, X47, X48, X49, X50, X51, X52, X53, X54) { } };
struct X56 { BOOST_DI_INJECT(X56, X46, X47, X48, X49, X50, X51, X52, X53, X54, X55) { } };
struct X57 { BOOST_DI_INJECT(X57, X47, X48, X49, X50, X51, X52, X53, X54, X55, X56) { } };
struct X58 { BOOST_DI_INJECT(X58, X48, X49, X50, X51, X52, X53, X54, X55, X56, X57) { } };
struct X59 { BOOST_DI_INJECT(X59, X49, X50, X51, X52, X53, X54, X55, X56, X57, X58) { } };
struct X60 { BOOST_DI_INJECT(X60, X50, X51, X52, X53, X54, X55, X56, X57, X58, X59) { } };
struct X61 { BOOST_DI_INJECT(X61, X51, X52, X53, X54, X55, X56, X57, X58, X59, X60) { } };
struct X62 { BOOST_DI_INJECT(X62, X52, X53, X54, X55, X56, X57, X58, X59, X60, X61) { } };
struct X63 { BOOST_DI_INJECT(X63, X53, X54, X55, X56, X57, X58, X59, X60, X61, X62) { } };
struct X64 { BOOST_DI_INJECT(X64, X54, X55, X56, X57, X58, X59, X60, X61, X62, X63) { } };
struct X65 { BOOST_DI_INJECT(X65, X55, X56, X57, X58, X59, X60, X61, X62, X63, X64) { } };
struct X66 { BOOST_DI_INJECT(X66, X56, X57, X58, X59, X60, X61, X62, X63, X64, X65) { } };
struct X67 { BOOST_DI_INJECT(X67, X57, X58, X59, X60, X61, X62, X63, X64, X65, X66) { } };
struct X68 { BOOST_DI_INJECT(X68, X58, X59, X60, X61, X62, X63, X64, X65, X66, X67) { } };
struct X69 { BOOST_DI_INJECT(X69, X59, X60, X61, X62, X63, X64, X65, X66, X67, X68) { } };
struct X70 { BOOST_DI_INJECT(X70, X60, X61, X62, X63, X64, X65, X66, X67, X68, X69) { } };
struct X71 { BOOST_DI_INJECT(X71, X61, X62, X63, X64, X65, X66, X67, X68, X69, X70) { } };
struct X72 { BOOST_DI_INJECT(X72, X62, X63, X64, X65, X66, X67, X68, X69, X70, X71) { } };
struct X73 { BOOST_DI_INJECT(X73, X63, X64, X65, X66, X67, X68, X69, X70, X71, X72) { } };
struct X74 { BOOST_DI_INJECT(X74, X64, X65, X66, X67, X68, X69, X70, X71, X72, X73) { } };
struct X75 { BOOST_DI_INJECT(X75, X65, X66, X67, X68, X69, X70, X71, X72, X73, X74) { } };
struct X76 { BOOST_DI_INJECT(X76, X66, X67, X68, X69, X70, X71, X72, X73, X74, X75) { } };
struct X77 { BOOST_DI_INJECT(X77, X67, X68, X69, X70, X71, X72, X73, X74, X75, X76) { } };
struct X78 { BOOST_DI_INJECT(X78, X68, X69, X70, X71, X72, X73, X74, X75, X76, X77) { } };
struct X79 { BOOST_DI_INJECT(X79, X69, X70, X71, X72, X73, X74, X75, X76, X77, X78) { } };
struct X80 { BOOST_DI_INJECT(X80, X70, X71, X72, X73, X74, X75, X76, X77, X78, X79) { } };
struct X81 { BOOST_DI_INJECT(X81, X71, X72, X73, X74, X75, X76, X77, X78, X79, X80) { } };
struct X82 { BOOST_DI_INJECT(X82, X72, X73, X74, X75, X76, X77, X78, X79, X80, X81) { } };
struct X83 { BOOST_DI_INJECT(X83, X73, X74, X75, X76, X77, X78, X79, X80, X81, X82) { } };
struct X84 { BOOST_DI_INJECT(X84, X74, X75, X76, X77, X78, X79, X80, X81, X82, X83) { } };
struct X85 { BOOST_DI_INJECT(X85, X75, X76, X77, X78, X79, X80, X81, X82, X83, X84) { } };
struct X86 { BOOST_DI_INJECT(X86, X76, X77, X78, X79, X80, X81, X82, X83, X84, X85) { } };
struct X87 { BOOST_DI_INJECT(X87, X77, X78, X79, X80, X81, X82, X83, X84, X85, X86) { } };
struct X88 { BOOST_DI_INJECT(X88, X78, X79, X80, X81, X82, X83, X84, X85, X86, X87) { } };
struct X89 { BOOST_DI_INJECT(X89, X79, X80, X81, X82, X83, X84, X85, X86, X87, X88) { } };
struct X90 { BOOST_DI_INJECT(X90, X80, X81, X82, X83, X84, X85, X86, X87, X88, X89) { } };
struct X91 { BOOST_DI_INJECT(X91, X81, X82, X83, X84, X85, X86, X87, X88, X89, X90) { } };
struct X92 { BOOST_DI_INJECT(X92, X82, X83, X84, X85, X86, X87, X88, X89, X90, X91) { } };
struct X93 { BOOST_DI_INJECT(X93, X83, X84, X85, X86, X87, X88, X89, X90, X91, X92) { } };
struct X94 { BOOST_DI_INJECT(X94, X84, X85, X86, X87, X88, X89, X90, X91, X92, X93) { } };
struct X95 { BOOST_DI_INJECT(X95, X85, X86, X87, X88, X89, X90, X91, X92, X93, X94) { } };
struct X96 { BOOST_DI_INJECT(X96, X86, X87, X88, X89, X90, X91, X92, X93, X94, X95) { } };
struct X97 { BOOST_DI_INJECT(X97, X87, X88, X89, X90, X91, X92, X93, X94, X95, X96) { } };
struct X98 { BOOST_DI_INJECT(X98, X88, X89, X90, X91, X92, X93, X94, X95, X96, X97) { } };
struct X99 { BOOST_DI_INJECT(X99, X89, X90, X91, X92, X93, X94, X95, X96, X97, X98) { } };
struct I00 { virtual ~I00() noexcept = default; virtual void dummy() = 0; }; struct Impl00 : I00 { BOOST_DI_INJECT(Impl00, X00, X01, X02, X03, X04, X05, X06, X07, X08, X09) { } void dummy() override { } };
struct I01 { virtual ~I01() noexcept = default; virtual void dummy() = 0; }; struct Impl01 : I01 { BOOST_DI_INJECT(Impl01, X01, X02, X03, X04, X05, X06, X07, X08, X09, X10) { } void dummy() override { } };
struct I02 { virtual ~I02() noexcept = default; virtual void dummy() = 0; }; struct Impl02 : I02 { BOOST_DI_INJECT(Impl02, X02, X03, X04, X05, X06, X07, X08, X09, X10, X11) { } void dummy() override { } };
struct I03 { virtual ~I03() noexcept = default; virtual void dummy() = 0; }; struct Impl03 : I03 { BOOST_DI_INJECT(Impl03, X03, X04, X05, X06, X07, X08, X09, X10, X11, X12) { } void dummy() override { } };
struct I04 { virtual ~I04() noexcept = default; virtual void dummy() = 0; }; struct Impl04 : I04 { BOOST_DI_INJECT(Impl04, X04, X05, X06, X07, X08, X09, X10, X11, X12, X13) { } void dummy() override { } };
struct I05 { virtual ~I05() noexcept = default; virtual void dummy() = 0; }; struct Impl05 : I05 { BOOST_DI_INJECT(Impl05, X05, X06, X07, X08, X09, X10, X11, X12, X13, X14) { } void dummy() override { } };
struct I06 { virtual ~I06() noexcept = default; virtual void dummy() = 0; }; struct Impl06 : I06 { BOOST_DI_INJECT(Impl06, X06, X07, X08, X09, X10, X11, X12, X13, X14, X15) { } void dummy() override { } };
struct I07 { virtual ~I07() noexcept = default; virtual void dummy() = 0; }; struct Impl07 : I07 { BOOST_DI_INJECT(Impl07, X07, X08, X09, X10, X11, X12, X13, X14, X15, X16) { } void dummy() override { } };
struct I08 { virtual ~I08() noexcept = default; virtual void dummy() = 0; }; struct Impl08 : I08 { BOOST_DI_INJECT(Impl08, X08, X09, X10, X11, X12, X13, X14, X15, X16, X17) { } void dummy() override { } };
struct I09 { virtual ~I09() noexcept = default; virtual void dummy() = 0; }; struct Impl09 : I09 { BOOST_DI_INJECT(Impl09, X09, X10, X11, X12, X13, X14, X15, X16, X17, X18) { } void dummy() override { } };
struct I10 { virtual ~I10() noexcept = default; virtual void dummy() = 0; }; struct Impl10 : I10 { BOOST_DI_INJECT(Impl10, X10, X11, X12, X13, X14, X15, X16, X17, X18, X19) { } void dummy() override { } };
struct I11 { virtual ~I11() noexcept = default; virtual void dummy() = 0; }; struct Impl11 : I11 { BOOST_DI_INJECT(Impl11, X11, X12, X13, X14, X15, X16, X17, X18, X19, X20) { } void dummy() override { } };
struct I12 { virtual ~I12() noexcept = default; virtual void dummy() = 0; }; struct Impl12 : I12 { BOOST_DI_INJECT(Impl12, X12, X13, X14, X15, X16, X17, X18, X19, X20, X21) { } void dummy() override { } };
struct I13 { virtual ~I13() noexcept = default; virtual void dummy() = 0; }; struct Impl13 : I13 { BOOST_DI_INJECT(Impl13, X13, X14, X15, X16, X17, X18, X19, X20, X21, X22) { } void dummy() override { } };
struct I14 { virtual ~I14() noexcept = default; virtual void dummy() = 0; }; struct Impl14 : I14 { BOOST_DI_INJECT(Impl14, X14, X15, X16, X17, X18, X19, X20, X21, X22, X23) { } void dummy() override { } };
struct I15 { virtual ~I15() noexcept = default; virtual void dummy() = 0; }; struct Impl15 : I15 { BOOST_DI_INJECT(Impl15, X15, X16, X17, X18, X19, X20, X21, X22, X23, X24) { } void dummy() override { } };
struct I16 { virtual ~I16() noexcept = default; virtual void dummy() = 0; }; struct Impl16 : I16 { BOOST_DI_INJECT(Impl16, X16, X17, X18, X19, X20, X21, X22, X23, X24, X25) { } void dummy() override { } };
struct I17 { virtual ~I17() noexcept = default; virtual void dummy() = 0; }; struct Impl17 : I17 { BOOST_DI_INJECT(Impl17, X17, X18, X19, X20, X21, X22, X23, X24, X25, X26) { } void dummy() override { } };
struct I18 { virtual ~I18() noexcept = default; virtual void dummy() = 0; }; struct Impl18 : I18 { BOOST_DI_INJECT(Impl18, X18, X19, X20, X21, X22, X23, X24, X25, X26, X27) { } void dummy() override { } };
struct I19 { virtual ~I19() noexcept = default; virtual void dummy() = 0; }; struct Impl19 : I19 { BOOST_DI_INJECT(Impl19, X19, X20, X21, X22, X23, X24, X25, X26, X27, X28) { } void dummy() override { } };
struct I20 { virtual ~I20() noexcept = default; virtual void dummy() = 0; }; struct Impl20 : I20 { BOOST_DI_INJECT(Impl20, X20, X21, X22, X23, X24, X25, X26, X27, X28, X29) { } void dummy() override { } };
struct I21 { virtual ~I21() noexcept = default; virtual void dummy() = 0; }; struct Impl21 : I21 { BOOST_DI_INJECT(Impl21, X21, X22, X23, X24, X25, X26, X27, X28, X29, X30) { } void dummy() override { } };
struct I22 { virtual ~I22() noexcept = default; virtual void dummy() = 0; }; struct Impl22 : I22 { BOOST_DI_INJECT(Impl22, X22, X23, X24, X25, X26, X27, X28, X29, X30, X31) { } void dummy() override { } };
struct I23 { virtual ~I23() noexcept = default; virtual void dummy() = 0; }; struct Impl23 : I23 { BOOST_DI_INJECT(Impl23, X23, X24, X25, X26, X27, X28, X29, X30, X31, X32) { } void dummy() override { } };
struct I24 { virtual ~I24() noexcept = default; virtual void dummy() = 0; }; struct Impl24 : I24 { BOOST_DI_INJECT(Impl24, X24, X25, X26, X27, X28, X29, X30, X31, X32, X33) { } void dummy() override { } };
struct I25 { virtual ~I25() noexcept = default; virtual void dummy() = 0; }; struct Impl25 : I25 { BOOST_DI_INJECT(Impl25, X25, X26, X27, X28, X29, X30, X31, X32, X33, X34) { } void dummy() override { } };
struct I26 { virtual ~I26() noexcept = default; virtual void dummy() = 0; }; struct Impl26 : I26 { BOOST_DI_INJECT(Impl26, X26, X27, X28, X29, X30, X31, X32, X33, X34, X35) { } void dummy() override { } };
struct I27 { virtual ~I27() noexcept = default; virtual void dummy() = 0; }; struct Impl27 : I27 { BOOST_DI_INJECT(Impl27, X27, X28, X29, X30, X31, X32, X33, X34, X35, X36) { } void dummy() override { } };
struct I28 { virtual ~I28() noexcept = default; virtual void dummy() = 0; }; struct Impl28 : I28 { BOOST_DI_INJECT(Impl28, X28, X29, X30, X31, X32, X33, X34, X35, X36, X37) { } void dummy() override { } };
struct I29 { virtual ~I29() noexcept = default; virtual void dummy() = 0; }; struct Impl29 : I29 { BOOST_DI_INJECT(Impl29, X29, X30, X31, X32, X33, X34, X35, X36, X37, X38) { } void dummy() override { } };
struct I30 { virtual ~I30() noexcept = default; virtual void dummy() = 0; }; struct Impl30 : I30 { BOOST_DI_INJECT(Impl30, X30, X31, X32, X33, X34, X35, X36, X37, X38, X39) { } void dummy() override { } };
struct I31 { virtual ~I31() noexcept = default; virtual void dummy() = 0; }; struct Impl31 : I31 { BOOST_DI_INJECT(Impl31, X31, X32, X33, X34, X35, X36, X37, X38, X39, X40) { } void dummy() override { } };
struct I32 { virtual ~I32() noexcept = default; virtual void dummy() = 0; }; struct Impl32 : I32 { BOOST_DI_INJECT(Impl32, X32, X33, X34, X35, X36, X37, X38, X39, X40, X41) { } void dummy() override { } };
struct I33 { virtual ~I33() noexcept = default; virtual void dummy() = 0; }; struct Impl33 : I33 { BOOST_DI_INJECT(Impl33, X33, X34, X35, X36, X37, X38, X39, X40, X41, X42) { } void dummy() override { } };
struct I34 { virtual ~I34() noexcept = default; virtual void dummy() = 0; }; struct Impl34 : I34 { BOOST_DI_INJECT(Impl34, X34, X35, X36, X37, X38, X39, X40, X41, X42, X43) { } void dummy() override { } };
struct I35 { virtual ~I35() noexcept = default; virtual void dummy() = 0; }; struct Impl35 : I35 { BOOST_DI_INJECT(Impl35, X35, X36, X37, X38, X39, X40, X41, X42, X43, X44) { } void dummy() override { } };
struct I36 { virtual ~I36() noexcept = default; virtual void dummy() = 0; }; struct Impl36 : I36 { BOOST_DI_INJECT(Impl36, X36, X37, X38, X39, X40, X41, X42, X43, X44, X45) { } void dummy() override { } };
struct I37 { virtual ~I37() noexcept = default; virtual void dummy() = 0; }; struct Impl37 : I37 { BOOST_DI_INJECT(Impl37, X37, X38, X39, X40, X41, X42, X43, X44, X45, X46) { } void dummy() override { } };
struct I38 { virtual ~I38() noexcept = default; virtual void dummy() = 0; }; struct Impl38 : I38 { BOOST_DI_INJECT(Impl38, X38, X39, X40, X41, X42, X43, X44, X45, X46, X47) { } void dummy() override { } };
struct I39 { virtual ~I39() noexcept = default; virtual void dummy() = 0; }; struct Impl39 : I39 { BOOST_DI_INJECT(Impl39, X39, X40, X41, X42, X43, X44, X45, X46, X47, X48) { } void dummy() override { } };
struct I40 { virtual ~I40() noexcept = default; virtual void dummy() = 0; }; struct Impl40 : I40 { BOOST_DI_INJECT(Impl40, X40, X41, X42, X43, X44, X45, X46, X47, X48, X49) { } void dummy() override { } };
struct I41 { virtual ~I41() noexcept = default; virtual void dummy() = 0; }; struct Impl41 : I41 { BOOST_DI_INJECT(Impl41, X41, X42, X43, X44, X45, X46, X47, X48, X49, X50) { } void dummy() override { } };
struct I42 { virtual ~I42() noexcept = default; virtual void dummy() = 0; }; struct Impl42 : I42 { BOOST_DI_INJECT(Impl42, X42, X43, X44, X45, X46, X47, X48, X49, X50, X51) { } void dummy() override { } };
struct I43 { virtual ~I43() noexcept = default; virtual void dummy() = 0; }; struct Impl43 : I43 { BOOST_DI_INJECT(Impl43, X43, X44, X45, X46, X47, X48, X49, X50, X51, X52) { } void dummy() override { } };
struct I44 { virtual ~I44() noexcept = default; virtual void dummy() = 0; }; struct Impl44 : I44 { BOOST_DI_INJECT(Impl44, X44, X45, X46, X47, X48, X49, X50, X51, X52, X53) { } void dummy() override { } };
struct I45 { virtual ~I45() noexcept = default; virtual void dummy() = 0; }; struct Impl45 : I45 { BOOST_DI_INJECT(Impl45, X45, X46, X47, X48, X49, X50, X51, X52, X53, X54) { } void dummy() override { } };
struct I46 { virtual ~I46() noexcept = default; virtual void dummy() = 0; }; struct Impl46 : I46 { BOOST_DI_INJECT(Impl46, X46, X47, X48, X49, X50, X51, X52, X53, X54, X55) { } void dummy() override { } };
struct I47 { virtual ~I47() noexcept = default; virtual void dummy() = 0; }; struct Impl47 : I47 { BOOST_DI_INJECT(Impl47, X47, X48, X49, X50, X51, X52, X53, X54, X55, X56) { } void dummy() override { } };
struct I48 { virtual ~I48() noexcept = default; virtual void dummy() = 0; }; struct Impl48 : I48 { BOOST_DI_INJECT(Impl48, X48, X49, X50, X51, X52, X53, X54, X55, X56, X57) { } void dummy() override { } };
struct I49 { virtual ~I49() noexcept = default; virtual void dummy() = 0; }; struct Impl49 : I49 { BOOST_DI_INJECT(Impl49, X49, X50, X51, X52, X53, X54, X55, X56, X57, X58) { } void dummy() override { } };
struct I50 { virtual ~I50() noexcept = default; virtual void dummy() = 0; }; struct Impl50 : I50 { BOOST_DI_INJECT(Impl50, X50, X51, X52, X53, X54, X55, X56, X57, X58, X59) { } void dummy() override { } };
struct I51 { virtual ~I51() noexcept = default; virtual void dummy() = 0; }; struct Impl51 : I51 { BOOST_DI_INJECT(Impl51, X51, X52, X53, X54, X55, X56, X57, X58, X59, X60) { } void dummy() override { } };
struct I52 { virtual ~I52() noexcept = default; virtual void dummy() = 0; }; struct Impl52 : I52 { BOOST_DI_INJECT(Impl52, X52, X53, X54, X55, X56, X57, X58, X59, X60, X61) { } void dummy() override { } };
struct I53 { virtual ~I53() noexcept = default; virtual void dummy() = 0; }; struct Impl53 : I53 { BOOST_DI_INJECT(Impl53, X53, X54, X55, X56, X57, X58, X59, X60, X61, X62) { } void dummy() override { } };
struct I54 { virtual ~I54() noexcept = default; virtual void dummy() = 0; }; struct Impl54 : I54 { BOOST_DI_INJECT(Impl54, X54, X55, X56, X57, X58, X59, X60, X61, X62, X63) { } void dummy() override { } };
struct I55 { virtual ~I55() noexcept = default; virtual void dummy() = 0; }; struct Impl55 : I55 { BOOST_DI_INJECT(Impl55, X55, X56, X57, X58, X59, X60, X61, X62, X63, X64) { } void dummy() override { } };
struct I56 { virtual ~I56() noexcept = default; virtual void dummy() = 0; }; struct Impl56 : I56 { BOOST_DI_INJECT(Impl56, X56, X57, X58, X59, X60, X61, X62, X63, X64, X65) { } void dummy() override { } };
struct I57 { virtual ~I57() noexcept = default; virtual void dummy() = 0; }; struct Impl57 : I57 { BOOST_DI_INJECT(Impl57, X57, X58, X59, X60, X61, X62, X63, X64, X65, X66) { } void dummy() override { } };
struct I58 { virtual ~I58() noexcept = default; virtual void dummy() = 0; }; struct Impl58 : I58 { BOOST_DI_INJECT(Impl58, X58, X59, X60, X61, X62, X63, X64, X65, X66, X67) { } void dummy() override { } };
struct I59 { virtual ~I59() noexcept = default; virtual void dummy() = 0; }; struct Impl59 : I59 { BOOST_DI_INJECT(Impl59, X59, X60, X61, X62, X63, X64, X65, X66, X67, X68) { } void dummy() override { } };
struct I60 { virtual ~I60() noexcept = default; virtual void dummy() = 0; }; struct Impl60 : I60 { BOOST_DI_INJECT(Impl60, X60, X61, X62, X63, X64, X65, X66, X67, X68, X69) { } void dummy() override { } };
struct I61 { virtual ~I61() noexcept = default; virtual void dummy() = 0; }; struct Impl61 : I61 { BOOST_DI_INJECT(Impl61, X61, X62, X63, X64, X65, X66, X67, X68, X69, X70) { } void dummy() override { } };
struct I62 { virtual ~I62() noexcept = default; virtual void dummy() = 0; }; struct Impl62 : I62 { BOOST_DI_INJECT(Impl62, X62, X63, X64, X65, X66, X67, X68, X69, X70, X71) { } void dummy() override { } };
struct I63 { virtual ~I63() noexcept = default; virtual void dummy() = 0; }; struct Impl63 : I63 { BOOST_DI_INJECT(Impl63, X63, X64, X65, X66, X67, X68, X69, X70, X71, X72) { } void dummy() override { } };
struct I64 { virtual ~I64() noexcept = default; virtual void dummy() = 0; }; struct Impl64 : I64 { BOOST_DI_INJECT(Impl64, X64, X65, X66, X67, X68, X69, X70, X71, X72, X73) { } void dummy() override { } };
struct I65 { virtual ~I65() noexcept = default; virtual void dummy() = 0; }; struct Impl65 : I65 { BOOST_DI_INJECT(Impl65, X65, X66, X67, X68, X69, X70, X71, X72, X73, X74) { } void dummy() override { } };
struct I66 { virtual ~I66() noexcept = default; virtual void dummy() = 0; }; struct Impl66 : I66 { BOOST_DI_INJECT(Impl66, X66, X67, X68, X69, X70, X71, X72, X73, X74, X75) { } void dummy() override { } };
struct I67 { virtual ~I67() noexcept = default; virtual void dummy() = 0; }; struct Impl67 : I67 { BOOST_DI_INJECT(Impl67, X67, X68, X69, X70, X71, X72, X73, X74, X75, X76) { } void dummy() override { } };
struct I68 { virtual ~I68() noexcept = default; virtual void dummy() = 0; }; struct Impl68 : I68 { BOOST_DI_INJECT(Impl68, X68, X69, X70, X71, X72, X73, X74, X75, X76, X77) { } void dummy() override { } };
struct I69 { virtual ~I69() noexcept = default; virtual void dummy() = 0; }; struct Impl69 : I69 { BOOST_DI_INJECT(Impl69, X69, X70, X71, X72, X73, X74, X75, X76, X77, X78) { } void dummy() override { } };
struct I70 { virtual ~I70() noexcept = default; virtual void dummy() = 0; }; struct Impl70 : I70 { BOOST_DI_INJECT(Impl70, X70, X71, X72, X73, X74, X75, X76, X77, X78, X79) { } void dummy() override { } };
struct I71 { virtual ~I71() noexcept = default; virtual void dummy() = 0; }; struct Impl71 : I71 { BOOST_DI_INJECT(Impl71, X71, X72, X73, X74, X75, X76, X77, X78, X79, X80) { } void dummy() override { } };
struct I72 { virtual ~I72() noexcept = default; virtual void dummy() = 0; }; struct Impl72 : I72 { BOOST_DI_INJECT(Impl72, X72, X73, X74, X75, X76, X77, X78, X79, X80, X81) { } void dummy() override { } };
struct I73 { virtual ~I73() noexcept = default; virtual void dummy() = 0; }; struct Impl73 : I73 { BOOST_DI_INJECT(Impl73, X73, X74, X75, X76, X77, X78, X79, X80, X81, X82) { } void dummy() override { } };
struct I74 { virtual ~I74() noexcept = default; virtual void dummy() = 0; }; struct Impl74 : I74 { BOOST_DI_INJECT(Impl74, X74, X75, X76, X77, X78, X79, X80, X81, X82, X83) { } void dummy() override { } };
struct I75 { virtual ~I75() noexcept = default; virtual void dummy() = 0; }; struct Impl75 : I75 { BOOST_DI_INJECT(Impl75, X75, X76, X77, X78, X79, X80, X81, X82, X83, X84) { } void dummy() override { } };
struct I76 { virtual ~I76() noexcept = default; virtual void dummy() = 0; }; struct Impl76 : I76 { BOOST_DI_INJECT(Impl76, X76, X77, X78, X79, X80, X81, X82, X83, X84, X85) { } void dummy() override { } };
struct I77 { virtual ~I77() noexcept = default; virtual void dummy() = 0; }; struct Impl77 : I77 { BOOST_DI_INJECT(Impl77, X77, X78, X79, X80, X81, X82, X83, X84, X85, X86) { } void dummy() override { } };
struct I78 { virtual ~I78() noexcept = default; virtual void dummy() = 0; }; struct Impl78 : I78 { BOOST_DI_INJECT(Impl78, X78, X79, X80, X81, X82, X83, X84, X85, X86, X87) { } void dummy() override { } };
struct I79 { virtual ~I79() noexcept = default; virtual void dummy() = 0; }; struct Impl79 : I79 { BOOST_DI_INJECT(Impl79, X79, X80, X81, X82, X83, X84, X85, X86, X87, X88) { } void dummy() override { } };
struct I80 { virtual ~I80() noexcept = default; virtual void dummy() = 0; }; struct Impl80 : I80 { BOOST_DI_INJECT(Impl80, X80, X81, X82, X83, X84, X85, X86, X87, X88, X89) { } void dummy() override { } };
struct I81 { virtual ~I81() noexcept = default; virtual void dummy() = 0; }; struct Impl81 : I81 { BOOST_DI_INJECT(Impl81, X81, X82, X83, X84, X85, X86, X87, X88, X89, X90) { } void dummy() override { } };
struct I82 { virtual ~I82() noexcept = default; virtual void dummy() = 0; }; struct Impl82 : I82 { BOOST_DI_INJECT(Impl82, X82, X83, X84, X85, X86, X87, X88, X89, X90, X91) { } void dummy() override { } };
struct I83 { virtual ~I83() noexcept = default; virtual void dummy() = 0; }; struct Impl83 : I83 { BOOST_DI_INJECT(Impl83, X83, X84, X85, X86, X87, X88, X89, X90, X91, X92) { } void dummy() override { } };
struct I84 { virtual ~I84() noexcept = default; virtual void dummy() = 0; }; struct Impl84 : I84 { BOOST_DI_INJECT(Impl84, X84, X85, X86, X87, X88, X89, X90, X91, X92, X93) { } void dummy() override { } };
struct I85 { virtual ~I85() noexcept = default; virtual void dummy() = 0; }; struct Impl85 : I85 { BOOST_DI_INJECT(Impl85, X85, X86, X87, X88, X89, X90, X91, X92, X93, X94) { } void dummy() override { } };
struct I86 { virtual ~I86() noexcept = default; virtual void dummy() = 0; }; struct Impl86 : I86 { BOOST_DI_INJECT(Impl86, X86, X87, X88, X89, X90, X91, X92, X93, X94, X95) { } void dummy() override { } };
struct I87 { virtual ~I87() noexcept = default; virtual void dummy() = 0; }; struct Impl87 : I87 { BOOST_DI_INJECT(Impl87, X87, X88, X89, X90, X91, X92, X93, X94, X95, X96) { } void dummy() override { } };
struct I88 { virtual ~I88() noexcept = default; virtual void dummy() = 0; }; struct Impl88 : I88 { BOOST_DI_INJECT(Impl88, X88, X89, X90, X91, X92, X93, X94, X95, X96, X97) { } void dummy() override { } };
struct I89 { virtual ~I89() noexcept = default; virtual void dummy() = 0; }; struct Impl89 : I89 { BOOST_DI_INJECT(Impl89, X89, X90, X91, X92, X93, X94, X95, X96, X97, X98) { } void dummy() override { } };
struct I90 { virtual ~I90() noexcept = default; virtual void dummy() = 0; }; struct Impl90 : I90 { BOOST_DI_INJECT(Impl90, X90, X91, X92, X93, X94, X95, X96, X97, X98, X99) { } void dummy() override { } };
struct I91 { virtual ~I91() noexcept = default; virtual void dummy() = 0; }; struct Impl91 : I91 { BOOST_DI_INJECT(Impl91, X91, X92, X93, X94, X95, X96, X97, X98, X99, X00) { } void dummy() override { } };
struct I92 { virtual ~I92() noexcept = default; virtual void dummy() = 0; }; struct Impl92 : I92 { BOOST_DI_INJECT(Impl92, X92, X93, X94, X95, X96, X97, X98, X99, X00, X01) { } void dummy() override { } };
struct I93 { virtual ~I93() noexcept = default; virtual void dummy() = 0; }; struct Impl93 : I93 { BOOST_DI_INJECT(Impl93, X93, X94, X95, X96, X97, X98, X99, X00, X01, X02) { } void dummy() override { } };
struct I94 { virtual ~I94() noexcept = default; virtual void dummy() = 0; }; struct Impl94 : I94 { BOOST_DI_INJECT(Impl94, X94, X95, X96, X97, X98, X99, X00, X01, X02, X03) { } void dummy() override { } };
struct I95 { virtual ~I95() noexcept = default; virtual void dummy() = 0; }; struct Impl95 : I95 { BOOST_DI_INJECT(Impl95, X95, X96, X97, X98, X99, X00, X01, X02, X03, X04) { } void dummy() override { } };
struct I96 { virtual ~I96() noexcept = default; virtual void dummy() = 0; }; struct Impl96 : I96 { BOOST_DI_INJECT(Impl96, X96, X97, X98, X99, X00, X01, X02, X03, X04, X05) { } void dummy() override { } };
struct I97 { virtual ~I97() noexcept = default; virtual void dummy() = 0; }; struct Impl97 : I97 { BOOST_DI_INJECT(Impl97, X97, X98, X99, X00, X01, X02, X03, X04, X05, X06) { } void dummy() override { } };
struct I98 { virtual ~I98() noexcept = default; virtual void dummy() = 0; }; struct Impl98 : I98 { BOOST_DI_INJECT(Impl98, X98, X99, X00, X01, X02, X03, X04, X05, X06, X07) { } void dummy() override { } };
struct I99 { virtual ~I99() noexcept = default; virtual void dummy() = 0; }; struct Impl99 : I99 { BOOST_DI_INJECT(Impl99, X99, X00, X01, X02, X03, X04, X05, X06, X07, X08) { } void dummy() override { } };
struct C0 { BOOST_DI_INJECT(C0, std::unique_ptr<I00>, std::unique_ptr<I01>, std::unique_ptr<I02>, std::unique_ptr<I03>, std::unique_ptr<I04>, std::unique_ptr<I05>, std::unique_ptr<I06>, std::unique_ptr<I07>, std::unique_ptr<I08>, std::unique_ptr<I09>) { } };
struct C1 { BOOST_DI_INJECT(C1, std::unique_ptr<I10>, std::unique_ptr<I11>, std::unique_ptr<I12>, std::unique_ptr<I13>, std::unique_ptr<I14>, std::unique_ptr<I15>, std::unique_ptr<I16>, std::unique_ptr<I17>, std::unique_ptr<I18>, std::unique_ptr<I19>) { } };
struct C2 { BOOST_DI_INJECT(C2, std::unique_ptr<I20>, std::unique_ptr<I21>, std::unique_ptr<I22>, std::unique_ptr<I23>, std::unique_ptr<I24>, std::unique_ptr<I25>, std::unique_ptr<I26>, std::unique_ptr<I27>, std::unique_ptr<I28>, std::unique_ptr<I29>) { } };
struct C3 { BOOST_DI_INJECT(C3, std::unique_ptr<I30>, std::unique_ptr<I31>, std::unique_ptr<I32>, std::unique_ptr<I33>, std::unique_ptr<I34>, std::unique_ptr<I35>, std::unique_ptr<I36>, std::unique_ptr<I37>, std::unique_ptr<I38>, std::unique_ptr<I39>) { } };
struct C4 { BOOST_DI_INJECT(C4, std::unique_ptr<I40>, std::unique_ptr<I41>, std::unique_ptr<I42>, std::unique_ptr<I43>, std::unique_ptr<I44>, std::unique_ptr<I45>, std::unique_ptr<I46>, std::unique_ptr<I47>, std::unique_ptr<I48>, std::unique_ptr<I49>) { } };
struct C5 { BOOST_DI_INJECT(C5, std::unique_ptr<I50>, std::unique_ptr<I51>, std::unique_ptr<I52>, std::unique_ptr<I53>, std::unique_ptr<I54>, std::unique_ptr<I55>, std::unique_ptr<I56>, std::unique_ptr<I57>, std::unique_ptr<I58>, std::unique_ptr<I59>) { } };
struct C6 { BOOST_DI_INJECT(C6, std::unique_ptr<I60>, std::unique_ptr<I61>, std::unique_ptr<I62>, std::unique_ptr<I63>, std::unique_ptr<I64>, std::unique_ptr<I65>, std::unique_ptr<I66>, std::unique_ptr<I67>, std::unique_ptr<I68>, std::unique_ptr<I69>) { } };
struct C7 { BOOST_DI_INJECT(C7, std::unique_ptr<I70>, std::unique_ptr<I71>, std::unique_ptr<I72>, std::unique_ptr<I73>, std::unique_ptr<I74>, std::unique_ptr<I75>, std::unique_ptr<I76>, std::unique_ptr<I77>, std::unique_ptr<I78>, std::unique_ptr<I79>) { } };
struct C8 { BOOST_DI_INJECT(C8, std::unique_ptr<I80>, std::unique_ptr<I81>, std::unique_ptr<I82>, std::unique_ptr<I83>, std::unique_ptr<I84>, std::unique_ptr<I85>, std::unique_ptr<I86>, std::unique_ptr<I87>, std::unique_ptr<I88>, std::unique_ptr<I89>) { } };
struct C9 { BOOST_DI_INJECT(C9, std::unique_ptr<I90>, std::unique_ptr<I91>, std::unique_ptr<I92>, std::unique_ptr<I93>, std::unique_ptr<I94>, std::unique_ptr<I95>, std::unique_ptr<I96>, std::unique_ptr<I97>, std::unique_ptr<I98>, std::unique_ptr<I99>) { } };
struct Complex { BOOST_DI_INJECT(Complex, C0, C1, C2, C3, C4, C5, C6, C7, C8, C9) { } };
// clang-format on

auto module = [] {
  // clang-format off
  auto i = di::make_injector(
    di::bind<I00>.to<Impl00>()
  , di::bind<I01>.to<Impl01>()
  , di::bind<I02>.to<Impl02>()
  , di::bind<I03>.to<Impl03>()
  , di::bind<I04>.to<Impl04>()
  , di::bind<I05>.to<Impl05>()
  , di::bind<I06>.to<Impl06>()
  , di::bind<I07>.to<Impl07>()
  , di::bind<I08>.to<Impl08>()
  , di::bind<I09>.to<Impl09>()
  , di::bind<I10>.to<Impl10>()
  , di::bind<I11>.to<Impl11>()
  , di::bind<I12>.to<Impl12>()
  , di::bind<I13>.to<Impl13>()
  , di::bind<I14>.to<Impl14>()
  , di::bind<I15>.to<Impl15>()
  , di::bind<I16>.to<Impl16>()
  , di::bind<I17>.to<Impl17>()
  , di::bind<I18>.to<Impl18>()
  , di::bind<I19>.to<Impl19>()
  , di::bind<I20>.to<Impl20>()
  , di::bind<I21>.to<Impl21>()
  , di::bind<I22>.to<Impl22>()
  , di::bind<I23>.to<Impl23>()
  , di::bind<I24>.to<Impl24>()
  , di::bind<I25>.to<Impl25>()
  , di::bind<I26>.to<Impl26>()
  , di::bind<I27>.to<Impl27>()
  , di::bind<I28>.to<Impl28>()
  , di::bind<I29>.to<Impl29>()
  , di::bind<I30>.to<Impl30>()
  , di::bind<I31>.to<Impl31>()
  , di::bind<I32>.to<Impl32>()
  , di::bind<I33>.to<Impl33>()
  , di::bind<I34>.to<Impl34>()
  , di::bind<I35>.to<Impl35>()
  , di::bind<I36>.to<Impl36>()
  , di::bind<I37>.to<Impl37>()
  , di::bind<I38>.to<Impl38>()
  , di::bind<I39>.to<Impl39>()
  , di::bind<I40>.to<Impl40>()
  , di::bind<I41>.to<Impl41>()
  , di::bind<I42>.to<Impl42>()
  , di::bind<I43>.to<Impl43>()
  , di::bind<I44>.to<Impl44>()
  , di::bind<I45>.to<Impl45>()
  , di::bind<I46>.to<Impl46>()
  , di::bind<I47>.to<Impl47>()
  , di::bind<I48>.to<Impl48>()
  , di::bind<I49>.to<Impl49>()
  , di::bind<I50>.to<Impl50>()
  , di::bind<I51>.to<Impl51>()
  , di::bind<I52>.to<Impl52>()
  , di::bind<I53>.to<Impl53>()
  , di::bind<I54>.to<Impl54>()
  , di::bind<I55>.to<Impl55>()
  , di::bind<I56>.to<Impl56>()
  , di::bind<I57>.to<Impl57>()
  , di::bind<I58>.to<Impl58>()
  , di::bind<I59>.to<Impl59>()
  , di::bind<I60>.to<Impl60>()
  , di::bind<I61>.to<Impl61>()
  , di::bind<I62>.to<Impl62>()
  , di::bind<I63>.to<Impl63>()
  , di::bind<I64>.to<Impl64>()
  , di::bind<I65>.to<Impl65>()
  , di::bind<I66>.to<Impl66>()
  , di::bind<I67>.to<Impl67>()
  , di::bind<I68>.to<Impl68>()
  , di::bind<I69>.to<Impl69>()
  , di::bind<I70>.to<Impl70>()
  , di::bind<I71>.to<Impl71>()
  , di::bind<I72>.to<Impl72>()
  , di::bind<I73>.to<Impl73>()
  , di::bind<I74>.to<Impl74>()
  , di::bind<I75>.to<Impl75>()
  , di::bind<I76>.to<Impl76>()
  , di::bind<I77>.to<Impl77>()
  , di::bind<I78>.to<Impl78>()
  , di::bind<I79>.to<Impl79>()
  , di::bind<I80>.to<Impl80>()
  , di::bind<I81>.to<Impl81>()
  , di::bind<I82>.to<Impl82>()
  , di::bind<I83>.to<Impl83>()
  , di::bind<I84>.to<Impl84>()
  , di::bind<I85>.to<Impl85>()
  , di::bind<I86>.to<Impl86>()
  , di::bind<I87>.to<Impl87>()
  , di::bind<I88>.to<Impl88>()
  , di::bind<I89>.to<Impl89>()
  , di::bind<I90>.to<Impl90>()
  , di::bind<I91>.to<Impl91>()
  , di::bind<I92>.to<Impl92>()
  , di::bind<I93>.to<Impl93>()
  , di::bind<I94>.to<Impl94>()
  , di::bind<I95>.to<Impl95>()
  , di::bind<I96>.to<Impl96>()
  , di::bind<I97>.to<Impl97>()
  , di::bind<I98>.to<Impl98>()
  , di::bind<I99>.to<Impl99>()
  );
  // clang-format off

  using injector_t = decltype(i);
  struct m : injector_t {
    using injector_t::injector_t;
  } injector{static_cast<injector_t &&>(i)};
  return injector;
};

int main() {
  auto injector = di::make_injector(module());
  injector.create<Complex>();
}
