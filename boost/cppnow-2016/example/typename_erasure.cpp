//
// Copyright (c) 2016 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/di.hpp>

namespace di = boost::di;

#if defined(TYPENAME_ERASURE)
static auto make_injector_impl = [](auto injector) {
  using injector_t = decltype(injector);
  struct i : injector_t {
    explicit i(injector_t &&other)
        : injector_t(static_cast<injector_t &&>(other)) {}
  };
  return i{static_cast<injector_t &&>(injector)};
};

template <class TConfig = BOOST_DI_CFG, class... TDeps,
          BOOST_DI_REQUIRES_MSG(
              di::concepts::boundable<di::aux::type_list<TDeps...>>) = 0,
          BOOST_DI_REQUIRES_MSG(di::concepts::configurable<TConfig>) = 0>
inline auto make_injector_test(TDeps... args) noexcept {

  return make_injector_impl(
      di::core::injector<
          TConfig,
          decltype(
              ((TConfig *)0)->policies((di::concepts::injector<TConfig> *)0)),
          TDeps...>{di::core::init{}, static_cast<TDeps &&>(args)...});
}
#else
template <class TConfig = BOOST_DI_CFG, class... TDeps,
          BOOST_DI_REQUIRES_MSG(
              di::concepts::boundable<di::aux::type_list<TDeps...>>) = 0,
          BOOST_DI_REQUIRES_MSG(di::concepts::configurable<TConfig>) = 0>
inline auto make_injector_test(TDeps... args) noexcept {
  return di::core::injector<
      TConfig,
      decltype(((TConfig *)0)->policies((di::concepts::injector<TConfig> *)0)),
      TDeps...>{di::core::init{}, static_cast<TDeps &&>(args)...};
}
#endif

// clang-format off
struct X0 { X0(){} };
struct X1 { X1(){} };
struct X2 { X2(){} };
struct X3 { X3(){} };
struct X4 { X4(){} };
struct X5 { X5(){} };
struct X6 { X6(){} };
struct X7 { X7(){} };
struct X8 { X8(){} };
struct X9 { X9(){} };
struct X10 { X10(){} };
struct X11 { X11(){} };
struct X12 { X12(){} };
struct X13 { X13(){} };
struct X14 { X14(){} };
struct X15 { X15(){} };
struct X16 { X16(){} };
struct X17 { X17(){} };
struct X18 { X18(){} };
struct X19 { X19(){} };
struct X20 { X20(){} };
struct X21 { X21(){} };
struct X22 { X22(){} };
struct X23 { X23(){} };
struct X24 { X24(){} };
struct X25 { X25(){} };
struct X26 { X26(){} };
struct X27 { X27(){} };
struct X28 { X28(){} };
struct X29 { X29(){} };
struct X30 { X30(){} };
struct X31 { X31(){} };
struct X32 { X32(){} };
struct X33 { X33(){} };
struct X34 { X34(){} };
struct X35 { X35(){} };
struct X36 { X36(){} };
struct X37 { X37(){} };
struct X38 { X38(){} };
struct X39 { X39(){} };
struct X40 { X40(){} };
struct X41 { X41(){} };
struct X42 { X42(){} };
struct X43 { X43(){} };
struct X44 { X44(){} };
struct X45 { X45(){} };
struct X46 { X46(){} };
struct X47 { X47(){} };
struct X48 { X48(){} };
struct X49 { X49(){} };
struct X50 { X50(){} };
struct X51 { X51(){} };
struct X52 { X52(){} };
struct X53 { X53(){} };
struct X54 { X54(){} };
struct X55 { X55(){} };
struct X56 { X56(){} };
struct X57 { X57(){} };
struct X58 { X58(){} };
struct X59 { X59(){} };
struct X60 { X60(){} };
struct X61 { X61(){} };
struct X62 { X62(){} };
struct X63 { X63(){} };
struct X64 { X64(){} };
struct X65 { X65(){} };
struct X66 { X66(){} };
struct X67 { X67(){} };
struct X68 { X68(){} };
struct X69 { X69(){} };
struct X70 { X70(){} };
struct X71 { X71(){} };
struct X72 { X72(){} };
struct X73 { X73(){} };
struct X74 { X74(){} };
struct X75 { X75(){} };
struct X76 { X76(){} };
struct X77 { X77(){} };
struct X78 { X78(){} };
struct X79 { X79(){} };
struct X80 { X80(){} };
struct X81 { X81(){} };
struct X82 { X82(){} };
struct X83 { X83(){} };
struct X84 { X84(){} };
struct X85 { X85(){} };
struct X86 { X86(){} };
struct X87 { X87(){} };
struct X88 { X88(){} };
struct X89 { X89(){} };
struct X90 { X90(){} };
struct X91 { X91(){} };
struct X92 { X92(){} };
struct X93 { X93(){} };
struct X94 { X94(){} };
struct X95 { X95(){} };
struct X96 { X96(){} };
struct X97 { X97(){} };
struct X98 { X98(){} };
struct X99 { X99(){} };
struct X100 { X100(X0, X12, X15, X26, X27, X45, X58, X63, X80, X83){} };
struct X101 { X101(X9, X23, X24, X39, X44, X49, X56, X61, X92, X93){} };
struct X102 { X102(X8, X11, X17, X30, X33, X45, X50, X53, X56, X95){} };
struct X103 { X103(X6, X19, X44, X61, X63, X64, X73, X89, X91, X96){} };
struct X104 { X104(X12, X18, X25, X32, X35, X72, X81, X88, X92, X101){} };
struct X105 { X105(X44, X47, X61, X67, X76, X79, X81, X88, X94, X103){} };
struct X106 { X106(X1, X6, X9, X22, X23, X33, X51, X72, X77, X82){} };
struct X107 { X107(X8, X38, X40, X65, X66, X74, X75, X86, X92, X105){} };
struct X108 { X108(X7, X9, X18, X48, X53, X55, X60, X67, X69, X98){} };
struct X109 { X109(X16, X32, X40, X49, X60, X68, X76, X88, X91, X107){} };
struct X110 { X110(X8, X19, X26, X27, X29, X65, X76, X83, X86, X98){} };
struct X111 { X111(X5, X29, X35, X38, X51, X54, X57, X62, X92){} };
struct X112 { X112(X1, X2, X39, X48, X49, X55, X65, X78, X80, X105){} };
struct X113 { X113(X1, X10, X12, X15, X17, X30, X79, X87, X96, X104){} };
struct X114 { X114(X0, X1, X17, X62, X64, X69, X80, X89, X92, X109){} };
struct X115 { X115(X6, X29, X52, X65, X70, X82, X89, X93, X98, X113){} };
struct X116 { X116(X13, X31, X39, X47, X48, X58, X63, X70, X77, X99){} };
struct X117 { X117(X16, X40, X55, X59, X78, X79, X83, X91, X100, X106){} };
struct X118 { X118(X4, X16, X34, X53, X69, X74, X94, X96, X105, X116){} };
struct X119 { X119(X0, X19, X20, X22, X60, X76, X78, X83, X105, X112){} };
struct X120 { X120(X12, X14, X23, X24, X54, X60, X65, X70, X86, X105){} };
struct X121 { X121(X1, X4, X12, X16, X21, X31, X36, X54, X57, X96){} };
struct X122 { X122(X12, X13, X26, X37, X42, X54, X57, X64, X94, X105){} };
struct X123 { X123(X41, X63, X77, X79, X81, X90, X97, X103, X114, X120){} };
struct X124 { X124(X0, X10, X15, X45, X52, X54, X86, X90, X109, X110){} };
struct X125 { X125(X8, X11, X28, X32, X44, X45, X48, X92, X94, X112){} };
struct X126 { X126(X0, X27, X48, X64, X69, X72, X79, X97, X109, X124){} };
struct X127 { X127(X2, X3, X14, X29, X55, X79, X86, X92, X115, X122){} };
struct X128 { X128(X10, X35, X63, X66, X87, X98, X100, X107, X117, X119){} };
struct X129 { X129(X1, X9, X10, X16, X29, X62, X98, X117, X125, X128){} };
struct X130 { X130(X6, X29, X53, X81, X89, X90, X93, X105, X106, X108){} };
struct X131 { X131(X3, X9, X20, X36, X42, X53, X77, X84, X89, X119){} };
struct X132 { X132(X38, X41, X47, X87, X90, X99, X107, X119, X120, X126){} };
struct X133 { X133(X8, X11, X18, X28, X43, X45, X62, X90, X107, X109){} };
struct X134 { X134(X7, X23, X56, X69, X77, X80, X81, X86, X101, X103){} };
struct X135 { X135(X6, X13, X50, X69, X75, X80, X82, X93, X98, X106){} };
struct X136 { X136(X16, X41, X49, X63, X66, X78, X90, X103, X117, X122){} };
struct X137 { X137(X20, X21, X29, X30, X52, X53, X60, X66, X91, X131){} };
struct X138 { X138(X1, X10, X51, X53, X54, X73, X90, X96, X114, X115){} };
struct X139 { X139(X1, X2, X6, X19, X21, X47, X73, X76, X109, X126){} };
struct X140 { X140(X7, X18, X25, X38, X46, X55, X67, X90, X92, X103){} };
struct X141 { X141(X0, X28, X36, X47, X63, X66, X79, X101, X102, X126){} };
struct X142 { X142(X2, X13, X23, X27, X40, X49, X85, X104, X108, X137){} };
struct X143 { X143(X27, X36, X65, X68, X74, X76, X80, X93, X114, X116){} };
struct X144 { X144(X30, X37, X43, X81, X89, X90, X121, X125, X138, X143){} };
struct X145 { X145(X9, X25, X59, X82, X95, X99, X100, X114, X133, X137){} };
struct X146 { X146(X16, X44, X72, X93, X114, X120, X134, X135, X138, X141){} };
struct X147 { X147(X20, X28, X57, X73, X84, X94, X101, X107, X129, X137){} };
struct X148 { X148(X1, X60, X61, X73, X83, X90, X104, X107, X140, X143){} };
struct X149 { X149(X9, X23, X34, X39, X55, X68, X94, X97, X105, X114){} };
struct X150 { X150(X1, X19, X28, X35, X40, X55, X117, X125, X139, X149){} };
struct X151 { X151(X29, X40, X42, X56, X75, X77, X90, X120, X126, X143){} };
struct X152 { X152(X21, X27, X36, X38, X50, X71, X77, X102, X132, X133){} };
struct X153 { X153(X16, X23, X30, X58, X69, X74, X92, X106, X123, X139){} };
struct X154 { X154(X13, X20, X23, X54, X72, X82, X85, X96, X100, X119){} };
struct X155 { X155(X29, X57, X67, X78, X82, X85, X113, X114, X116, X121){} };
struct X156 { X156(X2, X6, X44, X66, X77, X88, X101, X107, X108, X141){} };
struct X157 { X157(X23, X38, X55, X56, X65, X82, X100, X106, X110, X150){} };
struct X158 { X158(X71, X75, X101, X105, X109, X118, X119, X137, X146, X150){} };
struct X159 { X159(X4, X9, X27, X59, X84, X87, X116, X124, X132, X154){} };
struct X160 { X160(X44, X66, X68, X81, X90, X125, X134, X135, X137, X147){} };
struct X161 { X161(X9, X24, X42, X47, X73, X118, X126, X132, X148, X149){} };
struct X162 { X162(X1, X10, X21, X37, X56, X70, X102, X106, X113, X161){} };
struct X163 { X163(X11, X31, X59, X60, X93, X98, X126, X134, X144, X151){} };
struct X164 { X164(X17, X18, X36, X39, X76, X106, X108, X140, X142, X160){} };
struct X165 { X165(X1, X47, X92, X93, X94, X101, X102, X125, X133, X146){} };
struct X166 { X166(X38, X40, X70, X97, X126, X128, X129, X133, X147, X164){} };
struct X167 { X167(X0, X1, X18, X20, X53, X79, X114, X128, X146, X165){} };
struct X168 { X168(X4, X5, X9, X84, X88, X96, X133, X143, X150, X152){} };
struct X169 { X169(X10, X59, X70, X81, X83, X87, X95, X97, X152, X165){} };
struct X170 { X170(X23, X26, X35, X74, X92, X93, X99, X144, X152, X156){} };
struct X171 { X171(X1, X3, X77, X88, X89, X108, X110, X116, X135, X159){} };
struct X172 { X172(X38, X44, X51, X55, X66, X74, X94, X132, X135, X159){} };
struct X173 { X173(X12, X22, X59, X66, X73, X83, X89, X105, X126, X137){} };
struct X174 { X174(X2, X7, X61, X64, X72, X99, X141, X149, X153, X169){} };
struct X175 { X175(X10, X29, X31, X84, X94, X96, X112, X124, X166, X170){} };
struct X176 { X176(X26, X33, X64, X74, X82, X105, X124, X141, X161, X172){} };
struct X177 { X177(X13, X19, X65, X91, X119, X120, X129, X142, X143, X161){} };
struct X178 { X178(X47, X49, X68, X99, X110, X141, X151, X169, X170, X172){} };
struct X179 { X179(X20, X23, X26, X28, X71, X74, X95, X101, X136, X170){} };
struct X180 { X180(X3, X15, X57, X65, X70, X92, X109, X111, X134, X140){} };
struct X181 { X181(X10, X14, X22, X23, X112, X116, X118, X130, X134, X152){} };
struct X182 { X182(X22, X28, X33, X48, X83, X84, X89, X129, X171, X180){} };
struct X183 { X183(X28, X35, X50, X75, X79, X87, X101, X127, X142, X179){} };
struct X184 { X184(X58, X60, X90, X105, X130, X150, X156, X162, X170, X179){} };
struct X185 { X185(X22, X44, X48, X91, X109, X111, X112, X140, X160, X179){} };
struct X186 { X186(X19, X38, X39, X41, X52, X83, X91, X155, X179, X183){} };
struct X187 { X187(X6, X19, X38, X48, X55, X78, X84, X104, X116, X178){} };
struct X188 { X188(X0, X19, X23, X75, X151, X152, X158, X163, X174, X186){} };
struct X189 { X189(X24, X52, X57, X76, X89, X96, X126, X128, X136, X140){} };
struct X190 { X190(X14, X18, X28, X84, X94, X133, X146, X160, X176, X188){} };
struct X191 { X191(X25, X38, X75, X102, X104, X128, X133, X136, X155, X180){} };
struct X192 { X192(X6, X54, X85, X97, X127, X128, X132, X158, X169, X187){} };
struct X193 { X193(X17, X20, X25, X30, X55, X70, X84, X94, X121, X172){} };
struct X194 { X194(X3, X22, X88, X116, X142, X146, X151, X152, X171, X191){} };
struct X195 { X195(X17, X71, X76, X77, X113, X130, X155, X163, X171, X185){} };
struct X196 { X196(X62, X76, X106, X116, X117, X134, X153, X155, X177, X190){} };
struct X197 { X197(X19, X42, X61, X73, X99, X101, X106, X126, X159, X181){} };
struct X198 { X198(X4, X15, X33, X48, X62, X87, X97, X99, X148, X160){} };
struct X199 { X199(X17, X20, X33, X57, X67, X69, X80, X99, X122, X136){} };
// clang-format on

auto module = [] {
  // clang-format off
  return make_injector_test(
      di::bind<X0>.in(di::unique)
    , di::bind<X1>.in(di::unique)
    , di::bind<X2>.in(di::unique)
    , di::bind<X3>.in(di::unique)
    , di::bind<X4>.in(di::unique)
    , di::bind<X5>.in(di::unique)
    , di::bind<X6>.in(di::unique)
    , di::bind<X7>.in(di::unique)
    , di::bind<X8>.in(di::unique)
    , di::bind<X9>.in(di::unique)
    , di::bind<X10>.in(di::unique)
    , di::bind<X11>.in(di::unique)
    , di::bind<X12>.in(di::unique)
    , di::bind<X13>.in(di::unique)
    , di::bind<X14>.in(di::unique)
    , di::bind<X15>.in(di::unique)
    , di::bind<X16>.in(di::unique)
    , di::bind<X17>.in(di::unique)
    , di::bind<X18>.in(di::unique)
    , di::bind<X19>.in(di::unique)
    , di::bind<X20>.in(di::unique)
    , di::bind<X21>.in(di::unique)
    , di::bind<X22>.in(di::unique)
    , di::bind<X23>.in(di::unique)
    , di::bind<X24>.in(di::unique)
    , di::bind<X25>.in(di::unique)
    , di::bind<X26>.in(di::unique)
    , di::bind<X27>.in(di::unique)
    , di::bind<X28>.in(di::unique)
    , di::bind<X29>.in(di::unique)
    , di::bind<X30>.in(di::unique)
    , di::bind<X31>.in(di::unique)
    , di::bind<X32>.in(di::unique)
    , di::bind<X33>.in(di::unique)
    , di::bind<X34>.in(di::unique)
    , di::bind<X35>.in(di::unique)
    , di::bind<X36>.in(di::unique)
    , di::bind<X37>.in(di::unique)
    , di::bind<X38>.in(di::unique)
    , di::bind<X39>.in(di::unique)
    , di::bind<X40>.in(di::unique)
    , di::bind<X41>.in(di::unique)
    , di::bind<X42>.in(di::unique)
    , di::bind<X43>.in(di::unique)
    , di::bind<X44>.in(di::unique)
    , di::bind<X45>.in(di::unique)
    , di::bind<X46>.in(di::unique)
    , di::bind<X47>.in(di::unique)
    , di::bind<X48>.in(di::unique)
    , di::bind<X49>.in(di::unique)
    , di::bind<X50>.in(di::unique)
    , di::bind<X51>.in(di::unique)
    , di::bind<X52>.in(di::unique)
    , di::bind<X53>.in(di::unique)
    , di::bind<X54>.in(di::unique)
    , di::bind<X55>.in(di::unique)
    , di::bind<X56>.in(di::unique)
    , di::bind<X57>.in(di::unique)
    , di::bind<X58>.in(di::unique)
    , di::bind<X59>.in(di::unique)
    , di::bind<X60>.in(di::unique)
    , di::bind<X61>.in(di::unique)
    , di::bind<X62>.in(di::unique)
    , di::bind<X63>.in(di::unique)
    , di::bind<X64>.in(di::unique)
    , di::bind<X65>.in(di::unique)
    , di::bind<X66>.in(di::unique)
    , di::bind<X67>.in(di::unique)
    , di::bind<X68>.in(di::unique)
    , di::bind<X69>.in(di::unique)
    , di::bind<X70>.in(di::unique)
    , di::bind<X71>.in(di::unique)
    , di::bind<X72>.in(di::unique)
    , di::bind<X73>.in(di::unique)
    , di::bind<X74>.in(di::unique)
    , di::bind<X75>.in(di::unique)
    , di::bind<X76>.in(di::unique)
    , di::bind<X77>.in(di::unique)
    , di::bind<X78>.in(di::unique)
    , di::bind<X79>.in(di::unique)
    , di::bind<X80>.in(di::unique)
    , di::bind<X81>.in(di::unique)
    , di::bind<X82>.in(di::unique)
    , di::bind<X83>.in(di::unique)
    , di::bind<X84>.in(di::unique)
    , di::bind<X85>.in(di::unique)
    , di::bind<X86>.in(di::unique)
    , di::bind<X87>.in(di::unique)
    , di::bind<X88>.in(di::unique)
    , di::bind<X89>.in(di::unique)
    , di::bind<X90>.in(di::unique)
    , di::bind<X91>.in(di::unique)
    , di::bind<X92>.in(di::unique)
    , di::bind<X93>.in(di::unique)
    , di::bind<X94>.in(di::unique)
    , di::bind<X95>.in(di::unique)
    , di::bind<X96>.in(di::unique)
    , di::bind<X97>.in(di::unique)
    , di::bind<X98>.in(di::unique)
    , di::bind<X99>.in(di::unique)
    , di::bind<X100>.in(di::unique)
    , di::bind<X101>.in(di::unique)
    , di::bind<X102>.in(di::unique)
    , di::bind<X103>.in(di::unique)
    , di::bind<X104>.in(di::unique)
    , di::bind<X105>.in(di::unique)
    , di::bind<X106>.in(di::unique)
    , di::bind<X107>.in(di::unique)
    , di::bind<X108>.in(di::unique)
    , di::bind<X109>.in(di::unique)
    , di::bind<X110>.in(di::unique)
    , di::bind<X111>.in(di::unique)
    , di::bind<X112>.in(di::unique)
    , di::bind<X113>.in(di::unique)
    , di::bind<X114>.in(di::unique)
    , di::bind<X115>.in(di::unique)
    , di::bind<X116>.in(di::unique)
    , di::bind<X117>.in(di::unique)
    , di::bind<X118>.in(di::unique)
    , di::bind<X119>.in(di::unique)
    , di::bind<X120>.in(di::unique)
    , di::bind<X121>.in(di::unique)
    , di::bind<X122>.in(di::unique)
    , di::bind<X123>.in(di::unique)
    , di::bind<X124>.in(di::unique)
    , di::bind<X125>.in(di::unique)
    , di::bind<X126>.in(di::unique)
    , di::bind<X127>.in(di::unique)
    , di::bind<X128>.in(di::unique)
    , di::bind<X129>.in(di::unique)
    , di::bind<X130>.in(di::unique)
    , di::bind<X131>.in(di::unique)
    , di::bind<X132>.in(di::unique)
    , di::bind<X133>.in(di::unique)
    , di::bind<X134>.in(di::unique)
    , di::bind<X135>.in(di::unique)
    , di::bind<X136>.in(di::unique)
    , di::bind<X137>.in(di::unique)
    , di::bind<X138>.in(di::unique)
    , di::bind<X139>.in(di::unique)
    , di::bind<X140>.in(di::unique)
    , di::bind<X141>.in(di::unique)
    , di::bind<X142>.in(di::unique)
    , di::bind<X143>.in(di::unique)
    , di::bind<X144>.in(di::unique)
    , di::bind<X145>.in(di::unique)
    , di::bind<X146>.in(di::unique)
    , di::bind<X147>.in(di::unique)
    , di::bind<X148>.in(di::unique)
    , di::bind<X149>.in(di::unique)
    , di::bind<X150>.in(di::unique)
    , di::bind<X151>.in(di::unique)
    , di::bind<X152>.in(di::unique)
    , di::bind<X153>.in(di::unique)
    , di::bind<X154>.in(di::unique)
    , di::bind<X155>.in(di::unique)
    , di::bind<X156>.in(di::unique)
    , di::bind<X157>.in(di::unique)
    , di::bind<X158>.in(di::unique)
    , di::bind<X159>.in(di::unique)
    , di::bind<X160>.in(di::unique)
    , di::bind<X161>.in(di::unique)
    , di::bind<X162>.in(di::unique)
    , di::bind<X163>.in(di::unique)
    , di::bind<X164>.in(di::unique)
    , di::bind<X165>.in(di::unique)
    , di::bind<X166>.in(di::unique)
    , di::bind<X167>.in(di::unique)
    , di::bind<X168>.in(di::unique)
    , di::bind<X169>.in(di::unique)
    , di::bind<X170>.in(di::unique)
    , di::bind<X171>.in(di::unique)
    , di::bind<X172>.in(di::unique)
    , di::bind<X173>.in(di::unique)
    , di::bind<X174>.in(di::unique)
    , di::bind<X175>.in(di::unique)
    , di::bind<X176>.in(di::unique)
    , di::bind<X177>.in(di::unique)
    , di::bind<X178>.in(di::unique)
    , di::bind<X179>.in(di::unique)
    , di::bind<X180>.in(di::unique)
    , di::bind<X181>.in(di::unique)
    , di::bind<X182>.in(di::unique)
    , di::bind<X183>.in(di::unique)
    , di::bind<X184>.in(di::unique)
    , di::bind<X185>.in(di::unique)
    , di::bind<X186>.in(di::unique)
    , di::bind<X187>.in(di::unique)
    , di::bind<X188>.in(di::unique)
    , di::bind<X189>.in(di::unique)
    , di::bind<X190>.in(di::unique)
    , di::bind<X191>.in(di::unique)
    , di::bind<X192>.in(di::unique)
    , di::bind<X193>.in(di::unique)
    , di::bind<X194>.in(di::unique)
    , di::bind<X195>.in(di::unique)
    , di::bind<X196>.in(di::unique)
    , di::bind<X197>.in(di::unique)
    , di::bind<X198>.in(di::unique)
    , di::bind<X199>.in(di::unique)
  );
  // clang-format on
};

template <class> struct q;
int main() {
  auto injector = make_injector_test(module());

  injector.create<X0>();
  injector.create<X1>();
  injector.create<X2>();
  injector.create<X3>();
  injector.create<X4>();
  injector.create<X5>();
  injector.create<X6>();
  injector.create<X7>();
  injector.create<X8>();
  injector.create<X9>();
  injector.create<X10>();
  injector.create<X11>();
  injector.create<X12>();
  injector.create<X13>();
  injector.create<X14>();
  injector.create<X15>();
  injector.create<X16>();
  injector.create<X17>();
  injector.create<X18>();
  injector.create<X19>();
  injector.create<X20>();
  injector.create<X21>();
  injector.create<X22>();
  injector.create<X23>();
  injector.create<X24>();
  injector.create<X25>();
  injector.create<X26>();
  injector.create<X27>();
  injector.create<X28>();
  injector.create<X29>();
  injector.create<X30>();
  injector.create<X31>();
  injector.create<X32>();
  injector.create<X33>();
  injector.create<X34>();
  injector.create<X35>();
  injector.create<X36>();
  injector.create<X37>();
  injector.create<X38>();
  injector.create<X39>();
  injector.create<X40>();
  injector.create<X41>();
  injector.create<X42>();
  injector.create<X43>();
  injector.create<X44>();
  injector.create<X45>();
  injector.create<X46>();
  injector.create<X47>();
  injector.create<X48>();
  injector.create<X49>();
  injector.create<X50>();
  injector.create<X51>();
  injector.create<X52>();
  injector.create<X53>();
  injector.create<X54>();
  injector.create<X55>();
  injector.create<X56>();
  injector.create<X57>();
  injector.create<X58>();
  injector.create<X59>();
  injector.create<X60>();
  injector.create<X61>();
  injector.create<X62>();
  injector.create<X63>();
  injector.create<X64>();
  injector.create<X65>();
  injector.create<X66>();
  injector.create<X67>();
  injector.create<X68>();
  injector.create<X69>();
  injector.create<X70>();
  injector.create<X71>();
  injector.create<X72>();
  injector.create<X73>();
  injector.create<X74>();
  injector.create<X75>();
  injector.create<X76>();
  injector.create<X77>();
  injector.create<X78>();
  injector.create<X79>();
  injector.create<X80>();
  injector.create<X81>();
  injector.create<X82>();
  injector.create<X83>();
  injector.create<X84>();
  injector.create<X85>();
  injector.create<X86>();
  injector.create<X87>();
  injector.create<X88>();
  injector.create<X89>();
  injector.create<X90>();
  injector.create<X91>();
  injector.create<X92>();
  injector.create<X93>();
  injector.create<X94>();
  injector.create<X95>();
  injector.create<X96>();
  injector.create<X97>();
  injector.create<X98>();
  injector.create<X99>();
  injector.create<X100>();
  injector.create<X101>();
  injector.create<X102>();
  injector.create<X103>();
  injector.create<X104>();
  injector.create<X105>();
  injector.create<X106>();
  injector.create<X107>();
  injector.create<X108>();
  injector.create<X109>();
  injector.create<X110>();
  injector.create<X111>();
  injector.create<X112>();
  injector.create<X113>();
  injector.create<X114>();
  injector.create<X115>();
  injector.create<X116>();
  injector.create<X117>();
  injector.create<X118>();
  injector.create<X119>();
  injector.create<X120>();
  injector.create<X121>();
  injector.create<X122>();
  injector.create<X123>();
  injector.create<X124>();
  injector.create<X125>();
  injector.create<X126>();
  injector.create<X127>();
  injector.create<X128>();
  injector.create<X129>();
  injector.create<X130>();
  injector.create<X131>();
  injector.create<X132>();
  injector.create<X133>();
  injector.create<X134>();
  injector.create<X135>();
  injector.create<X136>();
  injector.create<X137>();
  injector.create<X138>();
  injector.create<X139>();
  injector.create<X140>();
  injector.create<X141>();
  injector.create<X142>();
  injector.create<X143>();
  injector.create<X144>();
  injector.create<X145>();
  injector.create<X146>();
  injector.create<X147>();
  injector.create<X148>();
  injector.create<X149>();
  injector.create<X150>();
  injector.create<X151>();
  injector.create<X152>();
  injector.create<X153>();
  injector.create<X154>();
  injector.create<X155>();
  injector.create<X156>();
  injector.create<X157>();
  injector.create<X158>();
  injector.create<X159>();
  injector.create<X160>();
  injector.create<X161>();
  injector.create<X162>();
  injector.create<X163>();
  injector.create<X164>();
  injector.create<X165>();
  injector.create<X166>();
  injector.create<X167>();
  injector.create<X168>();
  injector.create<X169>();
  injector.create<X170>();
  injector.create<X171>();
  injector.create<X172>();
  injector.create<X173>();
  injector.create<X174>();
  injector.create<X175>();
  injector.create<X176>();
  injector.create<X177>();
  injector.create<X178>();
  injector.create<X179>();
  injector.create<X180>();
  injector.create<X181>();
  injector.create<X182>();
  injector.create<X183>();
  injector.create<X184>();
  injector.create<X185>();
  injector.create<X186>();
  injector.create<X187>();
  injector.create<X188>();
  injector.create<X189>();
  injector.create<X190>();
  injector.create<X191>();
  injector.create<X192>();
  injector.create<X193>();
  injector.create<X194>();
  injector.create<X195>();
  injector.create<X196>();
  injector.create<X197>();
  injector.create<X198>();
  injector.create<X199>();
}
