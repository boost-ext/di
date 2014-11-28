#include <boost/di.hpp>

namespace di = boost::di;

#if 0
struct X0 {  };
struct X1 {  };
struct X2 {  };
struct X3 {  };
struct X4 {  };
struct X5 {  };
struct X6 {  };
struct X7 {  };
struct X8 {  };
struct X9 {  };
struct X10 {  };
struct X11 {  };
struct X12 {  };
struct X13 {  };
struct X14 {  };
struct X15 {  };
struct X16 {  };
struct X17 {  };
struct X18 {  };
struct X19 {  };
struct X20 {  };
struct X21 {  };
struct X22 {  };
struct X23 {  };
struct X24 {  };
struct X25 {  };
struct X26 {  };
struct X27 {  };
struct X28 {  };
struct X29 {  };
struct X30 {  };
struct X31 {  };
struct X32 {  };
struct X33 {  };
struct X34 {  };
struct X35 {  };
struct X36 {  };
struct X37 {  };
struct X38 {  };
struct X39 {  };
struct X40 {  };
struct X41 {  };
struct X42 {  };
struct X43 {  };
struct X44 {  };
struct X45 {  };
struct X46 {  };
struct X47 {  };
struct X48 {  };
struct X49 {  };
struct X50 {  };
struct X51 {  };
struct X52 {  };
struct X53 {  };
struct X54 {  };
struct X55 {  };
struct X56 {  };
struct X57 {  };
struct X58 {  };
struct X59 {  };
struct X60 {  };
struct X61 {  };
struct X62 {  };
struct X63 {  };
struct X64 {  };
struct X65 {  };
struct X66 {  };
struct X67 {  };
struct X68 {  };
struct X69 {  };
struct X70 {  };
struct X71 {  };
struct X72 {  };
struct X73 {  };
struct X74 {  };
struct X75 {  };
struct X76 {  };
struct X77 {  };
struct X78 {  };
struct X79 {  };
struct X80 {  };
struct X81 {  };
struct X82 {  };
struct X83 {  };
struct X84 {  };
struct X85 {  };
struct X86 {  };
struct X87 {  };
struct X88 {  };
struct X89 {  };
struct X90 {  };
struct X91 {  };
struct X92 {  };
struct X93 {  };
struct X94 {  };
struct X95 {  };
struct X96 {  };
struct X97 {  };
struct X98 {  };
struct X99 {  };
struct X100 { X100(X0, X12, X15, X26, X27, X45, X58, X63, X80, X83) {} };
struct X101 { X101(X9, X23, X24, X39, X44, X49, X56, X61, X92, X93) {} };
struct X102 { X102(X8, X11, X17, X30, X33, X45, X50, X53, X56, X95) {} };
struct X103 { X103(X6, X19, X44, X61, X63, X64, X73, X89, X91, X96) {} };
struct X104 { X104(X12, X18, X25, X32, X35, X72, X81, X88, X92, X101) {} };
struct X105 { X105(X44, X47, X61, X67, X76, X79, X81, X88, X94, X103) {} };
struct X106 { X106(X1, X6, X9, X22, X23, X33, X51, X72, X77, X82) {} };
struct X107 { X107(X8, X38, X40, X65, X66, X74, X75, X86, X92, X105) {} };
struct X108 { X108(X7, X9, X18, X48, X53, X55, X60, X67, X69, X98) {} };
struct X109 { X109(X16, X32, X40, X49, X60, X68, X76, X88, X91, X107) {} };
struct X110 { X110(X8, X19, X26, X27, X29, X65, X76, X83, X86, X98) {} };
struct X111 { X111(X5, X29, X35, X38, X51, X54, X57, X62, X92, X104) {} };
struct X112 { X112(X1, X2, X39, X48, X49, X55, X65, X78, X80, X105) {} };
struct X113 { X113(X1, X10, X12, X15, X17, X30, X79, X87, X96, X104) {} };
struct X114 { X114(X0, X1, X17, X62, X64, X69, X80, X89, X92, X109) {} };
struct X115 { X115(X6, X29, X52, X65, X70, X82, X89, X93, X98, X113) {} };
struct X116 { X116(X13, X31, X39, X47, X48, X58, X63, X70, X77, X99) {} };
struct X117 { X117(X16, X40, X55, X59, X78, X79, X83, X91, X100, X106) {} };
struct X118 { X118(X4, X16, X34, X53, X69, X74, X94, X96, X105, X116) {} };
struct X119 { X119(X0, X19, X20, X22, X60, X76, X78, X83, X105, X112) {} };
struct X120 { X120(X12, X14, X23, X24, X54, X60, X65, X70, X86, X105) {} };
struct X121 { X121(X1, X4, X12, X16, X21, X31, X36, X54, X57, X96) {} };
struct X122 { X122(X12, X13, X26, X37, X42, X54, X57, X64, X94, X105) {} };
struct X123 { X123(X41, X63, X77, X79, X81, X90, X97, X103, X114, X120) {} };
struct X124 { X124(X0, X10, X15, X45, X52, X54, X86, X90, X109, X110) {} };
struct X125 { X125(X8, X11, X28, X32, X44, X45, X48, X92, X94, X112) {} };
struct X126 { X126(X0, X27, X48, X64, X69, X72, X79, X97, X109, X124) {} };
struct X127 { X127(X2, X3, X14, X29, X55, X79, X86, X92, X115, X122) {} };
struct X128 { X128(X10, X35, X63, X66, X87, X98, X100, X107, X117, X119) {} };
struct X129 { X129(X1, X9, X10, X16, X29, X62, X98, X117, X125, X128) {} };
struct X130 { X130(X6, X29, X53, X81, X89, X90, X93, X105, X106, X108) {} };
struct X131 { X131(X3, X9, X20, X36, X42, X53, X77, X84, X89, X119) {} };
struct X132 { X132(X38, X41, X47, X87, X90, X99, X107, X119, X120, X126) {} };
struct X133 { X133(X8, X11, X18, X28, X43, X45, X62, X90, X107, X109) {} };
struct X134 { X134(X7, X23, X56, X69, X77, X80, X81, X86, X101, X103) {} };
struct X135 { X135(X6, X13, X50, X69, X75, X80, X82, X93, X98, X106) {} };
struct X136 { X136(X16, X41, X49, X63, X66, X78, X90, X103, X117, X122) {} };
struct X137 { X137(X20, X21, X29, X30, X52, X53, X60, X66, X91, X131) {} };
struct X138 { X138(X1, X10, X51, X53, X54, X73, X90, X96, X114, X115) {} };
struct X139 { X139(X1, X2, X6, X19, X21, X47, X73, X76, X109, X126) {} };
struct X140 { X140(X7, X18, X25, X38, X46, X55, X67, X90, X92, X103) {} };
struct X141 { X141(X0, X28, X36, X47, X63, X66, X79, X101, X102, X126) {} };
struct X142 { X142(X2, X13, X23, X27, X40, X49, X85, X104, X108, X137) {} };
struct X143 { X143(X27, X36, X65, X68, X74, X76, X80, X93, X114, X116) {} };
struct X144 { X144(X30, X37, X43, X81, X89, X90, X121, X125, X138, X143) {} };
struct X145 { X145(X9, X25, X59, X82, X95, X99, X100, X114, X133, X137) {} };
struct X146 { X146(X16, X44, X72, X93, X114, X120, X134, X135, X138, X141) {} };
struct X147 { X147(X20, X28, X57, X73, X84, X94, X101, X107, X129, X137) {} };
struct X148 { X148(X1, X60, X61, X73, X83, X90, X104, X107, X140, X143) {} };
struct X149 { X149(X9, X23, X34, X39, X55, X68, X94, X97, X105, X114) {} };
struct X150 { X150(X1, X19, X28, X35, X40, X55, X117, X125, X139, X149) {} };
struct X151 { X151(X29, X40, X42, X56, X75, X77, X90, X120, X126, X143) {} };
struct X152 { X152(X21, X27, X36, X38, X50, X71, X77, X102, X132, X133) {} };
struct X153 { X153(X16, X23, X30, X58, X69, X74, X92, X106, X123, X139) {} };
struct X154 { X154(X13, X20, X23, X54, X72, X82, X85, X96, X100, X119) {} };
struct X155 { X155(X29, X57, X67, X78, X82, X85, X113, X114, X116, X121) {} };
struct X156 { X156(X2, X6, X44, X66, X77, X88, X101, X107, X108, X141) {} };
struct X157 { X157(X23, X38, X55, X56, X65, X82, X100, X106, X110, X150) {} };
struct X158 { X158(X71, X75, X101, X105, X109, X118, X119, X137, X146, X150) {} };
struct X159 { X159(X4, X9, X27, X59, X84, X87, X116, X124, X132, X154) {} };
struct X160 { X160(X44, X66, X68, X81, X90, X125, X134, X135, X137, X147) {} };
struct X161 { X161(X9, X24, X42, X47, X73, X118, X126, X132, X148, X149) {} };
struct X162 { X162(X1, X10, X21, X37, X56, X70, X102, X106, X113, X161) {} };
struct X163 { X163(X11, X31, X59, X60, X93, X98, X126, X134, X144, X151) {} };
struct X164 { X164(X17, X18, X36, X39, X76, X106, X108, X140, X142, X160) {} };
struct X165 { X165(X1, X47, X92, X93, X94, X101, X102, X125, X133, X146) {} };
struct X166 { X166(X38, X40, X70, X97, X126, X128, X129, X133, X147, X164) {} };
struct X167 { X167(X0, X1, X18, X20, X53, X79, X114, X128, X146, X165) {} };
struct X168 { X168(X4, X5, X9, X84, X88, X96, X133, X143, X150, X152) {} };
struct X169 { X169(X10, X59, X70, X81, X83, X87, X95, X97, X152, X165) {} };
struct X170 { X170(X23, X26, X35, X74, X92, X93, X99, X144, X152, X156) {} };
struct X171 { X171(X1, X3, X77, X88, X89, X108, X110, X116, X135, X159) {} };
struct X172 { X172(X38, X44, X51, X55, X66, X74, X94, X132, X135, X159) {} };
struct X173 { X173(X12, X22, X59, X66, X73, X83, X89, X105, X126, X137) {} };
struct X174 { X174(X2, X7, X61, X64, X72, X99, X141, X149, X153, X169) {} };
struct X175 { X175(X10, X29, X31, X84, X94, X96, X112, X124, X166, X170) {} };
struct X176 { X176(X26, X33, X64, X74, X82, X105, X124, X141, X161, X172) {} };
struct X177 { X177(X13, X19, X65, X91, X119, X120, X129, X142, X143, X161) {} };
struct X178 { X178(X47, X49, X68, X99, X110, X141, X151, X169, X170, X172) {} };
struct X179 { X179(X20, X23, X26, X28, X71, X74, X95, X101, X136, X170) {} };
struct X180 { X180(X3, X15, X57, X65, X70, X92, X109, X111, X134, X140) {} };
struct X181 { X181(X10, X14, X22, X23, X112, X116, X118, X130, X134, X152) {} };
struct X182 { X182(X22, X28, X33, X48, X83, X84, X89, X129, X171, X180) {} };
struct X183 { X183(X28, X35, X50, X75, X79, X87, X101, X127, X142, X179) {} };
struct X184 { X184(X58, X60, X90, X105, X130, X150, X156, X162, X170, X179) {} };
struct X185 { X185(X22, X44, X48, X91, X109, X111, X112, X140, X160, X179) {} };
struct X186 { X186(X19, X38, X39, X41, X52, X83, X91, X155, X179, X183) {} };
struct X187 { X187(X6, X19, X38, X48, X55, X78, X84, X104, X116, X178) {} };
struct X188 { X188(X0, X19, X23, X75, X151, X152, X158, X163, X174, X186) {} };
struct X189 { X189(X24, X52, X57, X76, X89, X96, X126, X128, X136, X140) {} };
struct X190 { X190(X14, X18, X28, X84, X94, X133, X146, X160, X176, X188) {} };
struct X191 { X191(X25, X38, X75, X102, X104, X128, X133, X136, X155, X180) {} };
struct X192 { X192(X6, X54, X85, X97, X127, X128, X132, X158, X169, X187) {} };
struct X193 { X193(X17, X20, X25, X30, X55, X70, X84, X94, X121, X172) {} };
struct X194 { X194(X3, X22, X88, X116, X142, X146, X151, X152, X171, X191) {} };
struct X195 { X195(X17, X71, X76, X77, X113, X130, X155, X163, X171, X185) {} };
struct X196 { X196(X62, X76, X106, X116, X117, X134, X153, X155, X177, X190) {} };
struct X197 { X197(X19, X42, X61, X73, X99, X101, X106, X126, X159, X181) {} };
struct X198 { X198(X4, X15, X33, X48, X62, X87, X97, X99, X148, X160) {} };
struct X199 { X199(X17, X20, X33, X57, X67, X69, X80, X99, X122, X136) {} };
#else

struct X0 { BOOST_DI_INJECT(X0) {} };
struct X1 { BOOST_DI_INJECT(X1) {} };
struct X2 { BOOST_DI_INJECT(X2) {} };
struct X3 { BOOST_DI_INJECT(X3) {} };
struct X4 { BOOST_DI_INJECT(X4) {} };
struct X5 { BOOST_DI_INJECT(X5) {} };
struct X6 { BOOST_DI_INJECT(X6) {} };
struct X7 { BOOST_DI_INJECT(X7) {} };
struct X8 { BOOST_DI_INJECT(X8) {} };
struct X9 { BOOST_DI_INJECT(X9) {} };
struct X10 { BOOST_DI_INJECT(X10) {} };
struct X11 { BOOST_DI_INJECT(X11) {} };
struct X12 { BOOST_DI_INJECT(X12) {} };
struct X13 { BOOST_DI_INJECT(X13) {} };
struct X14 { BOOST_DI_INJECT(X14) {} };
struct X15 { BOOST_DI_INJECT(X15) {} };
struct X16 { BOOST_DI_INJECT(X16) {} };
struct X17 { BOOST_DI_INJECT(X17) {} };
struct X18 { BOOST_DI_INJECT(X18) {} };
struct X19 { BOOST_DI_INJECT(X19) {} };
struct X20 { BOOST_DI_INJECT(X20) {} };
struct X21 { BOOST_DI_INJECT(X21) {} };
struct X22 { BOOST_DI_INJECT(X22) {} };
struct X23 { BOOST_DI_INJECT(X23) {} };
struct X24 { BOOST_DI_INJECT(X24) {} };
struct X25 { BOOST_DI_INJECT(X25) {} };
struct X26 { BOOST_DI_INJECT(X26) {} };
struct X27 { BOOST_DI_INJECT(X27) {} };
struct X28 { BOOST_DI_INJECT(X28) {} };
struct X29 { BOOST_DI_INJECT(X29) {} };
struct X30 { BOOST_DI_INJECT(X30) {} };
struct X31 { BOOST_DI_INJECT(X31) {} };
struct X32 { BOOST_DI_INJECT(X32) {} };
struct X33 { BOOST_DI_INJECT(X33) {} };
struct X34 { BOOST_DI_INJECT(X34) {} };
struct X35 { BOOST_DI_INJECT(X35) {} };
struct X36 { BOOST_DI_INJECT(X36) {} };
struct X37 { BOOST_DI_INJECT(X37) {} };
struct X38 { BOOST_DI_INJECT(X38) {} };
struct X39 { BOOST_DI_INJECT(X39) {} };
struct X40 { BOOST_DI_INJECT(X40) {} };
struct X41 { BOOST_DI_INJECT(X41) {} };
struct X42 { BOOST_DI_INJECT(X42) {} };
struct X43 { BOOST_DI_INJECT(X43) {} };
struct X44 { BOOST_DI_INJECT(X44) {} };
struct X45 { BOOST_DI_INJECT(X45) {} };
struct X46 { BOOST_DI_INJECT(X46) {} };
struct X47 { BOOST_DI_INJECT(X47) {} };
struct X48 { BOOST_DI_INJECT(X48) {} };
struct X49 { BOOST_DI_INJECT(X49) {} };
struct X50 { BOOST_DI_INJECT(X50) {} };
struct X51 { BOOST_DI_INJECT(X51) {} };
struct X52 { BOOST_DI_INJECT(X52) {} };
struct X53 { BOOST_DI_INJECT(X53) {} };
struct X54 { BOOST_DI_INJECT(X54) {} };
struct X55 { BOOST_DI_INJECT(X55) {} };
struct X56 { BOOST_DI_INJECT(X56) {} };
struct X57 { BOOST_DI_INJECT(X57) {} };
struct X58 { BOOST_DI_INJECT(X58) {} };
struct X59 { BOOST_DI_INJECT(X59) {} };
struct X60 { BOOST_DI_INJECT(X60) {} };
struct X61 { BOOST_DI_INJECT(X61) {} };
struct X62 { BOOST_DI_INJECT(X62) {} };
struct X63 { BOOST_DI_INJECT(X63) {} };
struct X64 { BOOST_DI_INJECT(X64) {} };
struct X65 { BOOST_DI_INJECT(X65) {} };
struct X66 { BOOST_DI_INJECT(X66) {} };
struct X67 { BOOST_DI_INJECT(X67) {} };
struct X68 { BOOST_DI_INJECT(X68) {} };
struct X69 { BOOST_DI_INJECT(X69) {} };
struct X70 { BOOST_DI_INJECT(X70) {} };
struct X71 { BOOST_DI_INJECT(X71) {} };
struct X72 { BOOST_DI_INJECT(X72) {} };
struct X73 { BOOST_DI_INJECT(X73) {} };
struct X74 { BOOST_DI_INJECT(X74) {} };
struct X75 { BOOST_DI_INJECT(X75) {} };
struct X76 { BOOST_DI_INJECT(X76) {} };
struct X77 { BOOST_DI_INJECT(X77) {} };
struct X78 { BOOST_DI_INJECT(X78) {} };
struct X79 { BOOST_DI_INJECT(X79) {} };
struct X80 { BOOST_DI_INJECT(X80) {} };
struct X81 { BOOST_DI_INJECT(X81) {} };
struct X82 { BOOST_DI_INJECT(X82) {} };
struct X83 { BOOST_DI_INJECT(X83) {} };
struct X84 { BOOST_DI_INJECT(X84) {} };
struct X85 { BOOST_DI_INJECT(X85) {} };
struct X86 { BOOST_DI_INJECT(X86) {} };
struct X87 { BOOST_DI_INJECT(X87) {} };
struct X88 { BOOST_DI_INJECT(X88) {} };
struct X89 { BOOST_DI_INJECT(X89) {} };
struct X90 { BOOST_DI_INJECT(X90) {} };
struct X91 { BOOST_DI_INJECT(X91) {} };
struct X92 { BOOST_DI_INJECT(X92) {} };
struct X93 { BOOST_DI_INJECT(X93) {} };
struct X94 { BOOST_DI_INJECT(X94) {} };
struct X95 { BOOST_DI_INJECT(X95) {} };
struct X96 { BOOST_DI_INJECT(X96) {} };
struct X97 { BOOST_DI_INJECT(X97) {} };
struct X98 { BOOST_DI_INJECT(X98) {} };
struct X99 { BOOST_DI_INJECT(X99) {} };
struct X100 { BOOST_DI_INJECT(X100, X0, X12, X15, X26, X27, X45, X58, X63, X80, X83) {} };
struct X101 { BOOST_DI_INJECT(X101, X9, X23, X24, X39, X44, X49, X56, X61, X92, X93) {} };
struct X102 { BOOST_DI_INJECT(X102, X8, X11, X17, X30, X33, X45, X50, X53, X56, X95) {} };
struct X103 { BOOST_DI_INJECT(X103, X6, X19, X44, X61, X63, X64, X73, X89, X91, X96) {} };
struct X104 { BOOST_DI_INJECT(X104, X12, X18, X25, X32, X35, X72, X81, X88, X92, X101) {} };
struct X105 { BOOST_DI_INJECT(X105, X44, X47, X61, X67, X76, X79, X81, X88, X94, X103) {} };
struct X106 { BOOST_DI_INJECT(X106, X1, X6, X9, X22, X23, X33, X51, X72, X77, X82) {} };
struct X107 { BOOST_DI_INJECT(X107, X8, X38, X40, X65, X66, X74, X75, X86, X92, X105) {} };
struct X108 { BOOST_DI_INJECT(X108, X7, X9, X18, X48, X53, X55, X60, X67, X69, X98) {} };
struct X109 { BOOST_DI_INJECT(X109, X16, X32, X40, X49, X60, X68, X76, X88, X91, X107) {} };
struct X110 { BOOST_DI_INJECT(X110, X8, X19, X26, X27, X29, X65, X76, X83, X86, X98) {} };
struct X111 { BOOST_DI_INJECT(X111, X5, X29, X35, X38, X51, X54, X57, X62, X92, X104) {} };
struct X112 { BOOST_DI_INJECT(X112, X1, X2, X39, X48, X49, X55, X65, X78, X80, X105) {} };
struct X113 { BOOST_DI_INJECT(X113, X1, X10, X12, X15, X17, X30, X79, X87, X96, X104) {} };
struct X114 { BOOST_DI_INJECT(X114, X0, X1, X17, X62, X64, X69, X80, X89, X92, X109) {} };
struct X115 { BOOST_DI_INJECT(X115, X6, X29, X52, X65, X70, X82, X89, X93, X98, X113) {} };
struct X116 { BOOST_DI_INJECT(X116, X13, X31, X39, X47, X48, X58, X63, X70, X77, X99) {} };
struct X117 { BOOST_DI_INJECT(X117, X16, X40, X55, X59, X78, X79, X83, X91, X100, X106) {} };
struct X118 { BOOST_DI_INJECT(X118, X4, X16, X34, X53, X69, X74, X94, X96, X105, X116) {} };
struct X119 { BOOST_DI_INJECT(X119, X0, X19, X20, X22, X60, X76, X78, X83, X105, X112) {} };
struct X120 { BOOST_DI_INJECT(X120, X12, X14, X23, X24, X54, X60, X65, X70, X86, X105) {} };
struct X121 { BOOST_DI_INJECT(X121, X1, X4, X12, X16, X21, X31, X36, X54, X57, X96) {} };
struct X122 { BOOST_DI_INJECT(X122, X12, X13, X26, X37, X42, X54, X57, X64, X94, X105) {} };
struct X123 { BOOST_DI_INJECT(X123, X41, X63, X77, X79, X81, X90, X97, X103, X114, X120) {} };
struct X124 { BOOST_DI_INJECT(X124, X0, X10, X15, X45, X52, X54, X86, X90, X109, X110) {} };
struct X125 { BOOST_DI_INJECT(X125, X8, X11, X28, X32, X44, X45, X48, X92, X94, X112) {} };
struct X126 { BOOST_DI_INJECT(X126, X0, X27, X48, X64, X69, X72, X79, X97, X109, X124) {} };
struct X127 { BOOST_DI_INJECT(X127, X2, X3, X14, X29, X55, X79, X86, X92, X115, X122) {} };
struct X128 { BOOST_DI_INJECT(X128, X10, X35, X63, X66, X87, X98, X100, X107, X117, X119) {} };
struct X129 { BOOST_DI_INJECT(X129, X1, X9, X10, X16, X29, X62, X98, X117, X125, X128) {} };
struct X130 { BOOST_DI_INJECT(X130, X6, X29, X53, X81, X89, X90, X93, X105, X106, X108) {} };
struct X131 { BOOST_DI_INJECT(X131, X3, X9, X20, X36, X42, X53, X77, X84, X89, X119) {} };
struct X132 { BOOST_DI_INJECT(X132, X38, X41, X47, X87, X90, X99, X107, X119, X120, X126) {} };
struct X133 { BOOST_DI_INJECT(X133, X8, X11, X18, X28, X43, X45, X62, X90, X107, X109) {} };
struct X134 { BOOST_DI_INJECT(X134, X7, X23, X56, X69, X77, X80, X81, X86, X101, X103) {} };
struct X135 { BOOST_DI_INJECT(X135, X6, X13, X50, X69, X75, X80, X82, X93, X98, X106) {} };
struct X136 { BOOST_DI_INJECT(X136, X16, X41, X49, X63, X66, X78, X90, X103, X117, X122) {} };
struct X137 { BOOST_DI_INJECT(X137, X20, X21, X29, X30, X52, X53, X60, X66, X91, X131) {} };
struct X138 { BOOST_DI_INJECT(X138, X1, X10, X51, X53, X54, X73, X90, X96, X114, X115) {} };
struct X139 { BOOST_DI_INJECT(X139, X1, X2, X6, X19, X21, X47, X73, X76, X109, X126) {} };
struct X140 { BOOST_DI_INJECT(X140, X7, X18, X25, X38, X46, X55, X67, X90, X92, X103) {} };
struct X141 { BOOST_DI_INJECT(X141, X0, X28, X36, X47, X63, X66, X79, X101, X102, X126) {} };
struct X142 { BOOST_DI_INJECT(X142, X2, X13, X23, X27, X40, X49, X85, X104, X108, X137) {} };
struct X143 { BOOST_DI_INJECT(X143, X27, X36, X65, X68, X74, X76, X80, X93, X114, X116) {} };
struct X144 { BOOST_DI_INJECT(X144, X30, X37, X43, X81, X89, X90, X121, X125, X138, X143) {} };
struct X145 { BOOST_DI_INJECT(X145, X9, X25, X59, X82, X95, X99, X100, X114, X133, X137) {} };
struct X146 { BOOST_DI_INJECT(X146, X16, X44, X72, X93, X114, X120, X134, X135, X138, X141) {} };
struct X147 { BOOST_DI_INJECT(X147, X20, X28, X57, X73, X84, X94, X101, X107, X129, X137) {} };
struct X148 { BOOST_DI_INJECT(X148, X1, X60, X61, X73, X83, X90, X104, X107, X140, X143) {} };
struct X149 { BOOST_DI_INJECT(X149, X9, X23, X34, X39, X55, X68, X94, X97, X105, X114) {} };
struct X150 { BOOST_DI_INJECT(X150, X1, X19, X28, X35, X40, X55, X117, X125, X139, X149) {} };
struct X151 { BOOST_DI_INJECT(X151, X29, X40, X42, X56, X75, X77, X90, X120, X126, X143) {} };
struct X152 { BOOST_DI_INJECT(X152, X21, X27, X36, X38, X50, X71, X77, X102, X132, X133) {} };
struct X153 { BOOST_DI_INJECT(X153, X16, X23, X30, X58, X69, X74, X92, X106, X123, X139) {} };
struct X154 { BOOST_DI_INJECT(X154, X13, X20, X23, X54, X72, X82, X85, X96, X100, X119) {} };
struct X155 { BOOST_DI_INJECT(X155, X29, X57, X67, X78, X82, X85, X113, X114, X116, X121) {} };
struct X156 { BOOST_DI_INJECT(X156, X2, X6, X44, X66, X77, X88, X101, X107, X108, X141) {} };
struct X157 { BOOST_DI_INJECT(X157, X23, X38, X55, X56, X65, X82, X100, X106, X110, X150) {} };
struct X158 { BOOST_DI_INJECT(X158, X71, X75, X101, X105, X109, X118, X119, X137, X146, X150) {} };
struct X159 { BOOST_DI_INJECT(X159, X4, X9, X27, X59, X84, X87, X116, X124, X132, X154) {} };
struct X160 { BOOST_DI_INJECT(X160, X44, X66, X68, X81, X90, X125, X134, X135, X137, X147) {} };
struct X161 { BOOST_DI_INJECT(X161, X9, X24, X42, X47, X73, X118, X126, X132, X148, X149) {} };
struct X162 { BOOST_DI_INJECT(X162, X1, X10, X21, X37, X56, X70, X102, X106, X113, X161) {} };
struct X163 { BOOST_DI_INJECT(X163, X11, X31, X59, X60, X93, X98, X126, X134, X144, X151) {} };
struct X164 { BOOST_DI_INJECT(X164, X17, X18, X36, X39, X76, X106, X108, X140, X142, X160) {} };
struct X165 { BOOST_DI_INJECT(X165, X1, X47, X92, X93, X94, X101, X102, X125, X133, X146) {} };
struct X166 { BOOST_DI_INJECT(X166, X38, X40, X70, X97, X126, X128, X129, X133, X147, X164) {} };
struct X167 { BOOST_DI_INJECT(X167, X0, X1, X18, X20, X53, X79, X114, X128, X146, X165) {} };
struct X168 { BOOST_DI_INJECT(X168, X4, X5, X9, X84, X88, X96, X133, X143, X150, X152) {} };
struct X169 { BOOST_DI_INJECT(X169, X10, X59, X70, X81, X83, X87, X95, X97, X152, X165) {} };
struct X170 { BOOST_DI_INJECT(X170, X23, X26, X35, X74, X92, X93, X99, X144, X152, X156) {} };
struct X171 { BOOST_DI_INJECT(X171, X1, X3, X77, X88, X89, X108, X110, X116, X135, X159) {} };
struct X172 { BOOST_DI_INJECT(X172, X38, X44, X51, X55, X66, X74, X94, X132, X135, X159) {} };
struct X173 { BOOST_DI_INJECT(X173, X12, X22, X59, X66, X73, X83, X89, X105, X126, X137) {} };
struct X174 { BOOST_DI_INJECT(X174, X2, X7, X61, X64, X72, X99, X141, X149, X153, X169) {} };
struct X175 { BOOST_DI_INJECT(X175, X10, X29, X31, X84, X94, X96, X112, X124, X166, X170) {} };
struct X176 { BOOST_DI_INJECT(X176, X26, X33, X64, X74, X82, X105, X124, X141, X161, X172) {} };
struct X177 { BOOST_DI_INJECT(X177, X13, X19, X65, X91, X119, X120, X129, X142, X143, X161) {} };
struct X178 { BOOST_DI_INJECT(X178, X47, X49, X68, X99, X110, X141, X151, X169, X170, X172) {} };
struct X179 { BOOST_DI_INJECT(X179, X20, X23, X26, X28, X71, X74, X95, X101, X136, X170) {} };
struct X180 { BOOST_DI_INJECT(X180, X3, X15, X57, X65, X70, X92, X109, X111, X134, X140) {} };
struct X181 { BOOST_DI_INJECT(X181, X10, X14, X22, X23, X112, X116, X118, X130, X134, X152) {} };
struct X182 { BOOST_DI_INJECT(X182, X22, X28, X33, X48, X83, X84, X89, X129, X171, X180) {} };
struct X183 { BOOST_DI_INJECT(X183, X28, X35, X50, X75, X79, X87, X101, X127, X142, X179) {} };
struct X184 { BOOST_DI_INJECT(X184, X58, X60, X90, X105, X130, X150, X156, X162, X170, X179) {} };
struct X185 { BOOST_DI_INJECT(X185, X22, X44, X48, X91, X109, X111, X112, X140, X160, X179) {} };
struct X186 { BOOST_DI_INJECT(X186, X19, X38, X39, X41, X52, X83, X91, X155, X179, X183) {} };
struct X187 { BOOST_DI_INJECT(X187, X6, X19, X38, X48, X55, X78, X84, X104, X116, X178) {} };
struct X188 { BOOST_DI_INJECT(X188, X0, X19, X23, X75, X151, X152, X158, X163, X174, X186) {} };
struct X189 { BOOST_DI_INJECT(X189, X24, X52, X57, X76, X89, X96, X126, X128, X136, X140) {} };
struct X190 { BOOST_DI_INJECT(X190, X14, X18, X28, X84, X94, X133, X146, X160, X176, X188) {} };
struct X191 { BOOST_DI_INJECT(X191, X25, X38, X75, X102, X104, X128, X133, X136, X155, X180) {} };
struct X192 { BOOST_DI_INJECT(X192, X6, X54, X85, X97, X127, X128, X132, X158, X169, X187) {} };
struct X193 { BOOST_DI_INJECT(X193, X17, X20, X25, X30, X55, X70, X84, X94, X121, X172) {} };
struct X194 { BOOST_DI_INJECT(X194, X3, X22, X88, X116, X142, X146, X151, X152, X171, X191) {} };
struct X195 { BOOST_DI_INJECT(X195, X17, X71, X76, X77, X113, X130, X155, X163, X171, X185) {} };
struct X196 { BOOST_DI_INJECT(X196, X62, X76, X106, X116, X117, X134, X153, X155, X177, X190) {} };
struct X197 { BOOST_DI_INJECT(X197, X19, X42, X61, X73, X99, X101, X106, X126, X159, X181) {} };
struct X198 { BOOST_DI_INJECT(X198, X4, X15, X33, X48, X62, X87, X97, X99, X148, X160) {} };
struct X199 { BOOST_DI_INJECT(X199, X17, X20, X33, X57, X67, X69, X80, X99, X122, X136) {} };


#endif

int main() {
long long sum = 0;

#if 1
auto injector = di::make_injector(
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
#endif

for (int i = 0; i < 1000; i++) {
#if 1
sum+=(injector.create<X0*>() != nullptr);
sum+=(injector.create<X1*>() != nullptr);
sum+=(injector.create<X2*>() != nullptr);
sum+=(injector.create<X3*>() != nullptr);
sum+=(injector.create<X4*>() != nullptr);
sum+=(injector.create<X5*>() != nullptr);
sum+=(injector.create<X6*>() != nullptr);
sum+=(injector.create<X7*>() != nullptr);
sum+=(injector.create<X8*>() != nullptr);
sum+=(injector.create<X9*>() != nullptr);
sum+=(injector.create<X10*>() != nullptr);
sum+=(injector.create<X11*>() != nullptr);
sum+=(injector.create<X12*>() != nullptr);
sum+=(injector.create<X13*>() != nullptr);
sum+=(injector.create<X14*>() != nullptr);
sum+=(injector.create<X15*>() != nullptr);
sum+=(injector.create<X16*>() != nullptr);
sum+=(injector.create<X17*>() != nullptr);
sum+=(injector.create<X18*>() != nullptr);
sum+=(injector.create<X19*>() != nullptr);
sum+=(injector.create<X20*>() != nullptr);
sum+=(injector.create<X21*>() != nullptr);
sum+=(injector.create<X22*>() != nullptr);
sum+=(injector.create<X23*>() != nullptr);
sum+=(injector.create<X24*>() != nullptr);
sum+=(injector.create<X25*>() != nullptr);
sum+=(injector.create<X26*>() != nullptr);
sum+=(injector.create<X27*>() != nullptr);
sum+=(injector.create<X28*>() != nullptr);
sum+=(injector.create<X29*>() != nullptr);
sum+=(injector.create<X30*>() != nullptr);
sum+=(injector.create<X31*>() != nullptr);
sum+=(injector.create<X32*>() != nullptr);
sum+=(injector.create<X33*>() != nullptr);
sum+=(injector.create<X34*>() != nullptr);
sum+=(injector.create<X35*>() != nullptr);
sum+=(injector.create<X36*>() != nullptr);
sum+=(injector.create<X37*>() != nullptr);
sum+=(injector.create<X38*>() != nullptr);
sum+=(injector.create<X39*>() != nullptr);
sum+=(injector.create<X40*>() != nullptr);
sum+=(injector.create<X41*>() != nullptr);
sum+=(injector.create<X42*>() != nullptr);
sum+=(injector.create<X43*>() != nullptr);
sum+=(injector.create<X44*>() != nullptr);
sum+=(injector.create<X45*>() != nullptr);
sum+=(injector.create<X46*>() != nullptr);
sum+=(injector.create<X47*>() != nullptr);
sum+=(injector.create<X48*>() != nullptr);
sum+=(injector.create<X49*>() != nullptr);
sum+=(injector.create<X50*>() != nullptr);
sum+=(injector.create<X51*>() != nullptr);
sum+=(injector.create<X52*>() != nullptr);
sum+=(injector.create<X53*>() != nullptr);
sum+=(injector.create<X54*>() != nullptr);
sum+=(injector.create<X55*>() != nullptr);
sum+=(injector.create<X56*>() != nullptr);
sum+=(injector.create<X57*>() != nullptr);
sum+=(injector.create<X58*>() != nullptr);
sum+=(injector.create<X59*>() != nullptr);
sum+=(injector.create<X60*>() != nullptr);
sum+=(injector.create<X61*>() != nullptr);
sum+=(injector.create<X62*>() != nullptr);
sum+=(injector.create<X63*>() != nullptr);
sum+=(injector.create<X64*>() != nullptr);
sum+=(injector.create<X65*>() != nullptr);
sum+=(injector.create<X66*>() != nullptr);
sum+=(injector.create<X67*>() != nullptr);
sum+=(injector.create<X68*>() != nullptr);
sum+=(injector.create<X69*>() != nullptr);
sum+=(injector.create<X70*>() != nullptr);
sum+=(injector.create<X71*>() != nullptr);
sum+=(injector.create<X72*>() != nullptr);
sum+=(injector.create<X73*>() != nullptr);
sum+=(injector.create<X74*>() != nullptr);
sum+=(injector.create<X75*>() != nullptr);
sum+=(injector.create<X76*>() != nullptr);
sum+=(injector.create<X77*>() != nullptr);
sum+=(injector.create<X78*>() != nullptr);
sum+=(injector.create<X79*>() != nullptr);
sum+=(injector.create<X80*>() != nullptr);
sum+=(injector.create<X81*>() != nullptr);
sum+=(injector.create<X82*>() != nullptr);
sum+=(injector.create<X83*>() != nullptr);
sum+=(injector.create<X84*>() != nullptr);
sum+=(injector.create<X85*>() != nullptr);
sum+=(injector.create<X86*>() != nullptr);
sum+=(injector.create<X87*>() != nullptr);
sum+=(injector.create<X88*>() != nullptr);
sum+=(injector.create<X89*>() != nullptr);
sum+=(injector.create<X90*>() != nullptr);
sum+=(injector.create<X91*>() != nullptr);
sum+=(injector.create<X92*>() != nullptr);
sum+=(injector.create<X93*>() != nullptr);
sum+=(injector.create<X94*>() != nullptr);
sum+=(injector.create<X95*>() != nullptr);
sum+=(injector.create<X96*>() != nullptr);
sum+=(injector.create<X97*>() != nullptr);
sum+=(injector.create<X98*>() != nullptr);
sum+=(injector.create<X99*>() != nullptr);
sum+=(injector.create<X100*>() != nullptr);
sum+=(injector.create<X101*>() != nullptr);
sum+=(injector.create<X102*>() != nullptr);
sum+=(injector.create<X103*>() != nullptr);
sum+=(injector.create<X104*>() != nullptr);
sum+=(injector.create<X105*>() != nullptr);
sum+=(injector.create<X106*>() != nullptr);
sum+=(injector.create<X107*>() != nullptr);
sum+=(injector.create<X108*>() != nullptr);
sum+=(injector.create<X109*>() != nullptr);
sum+=(injector.create<X110*>() != nullptr);
sum+=(injector.create<X111*>() != nullptr);
sum+=(injector.create<X112*>() != nullptr);
sum+=(injector.create<X113*>() != nullptr);
sum+=(injector.create<X114*>() != nullptr);
sum+=(injector.create<X115*>() != nullptr);
sum+=(injector.create<X116*>() != nullptr);
sum+=(injector.create<X117*>() != nullptr);
sum+=(injector.create<X118*>() != nullptr);
sum+=(injector.create<X119*>() != nullptr);
sum+=(injector.create<X120*>() != nullptr);
sum+=(injector.create<X121*>() != nullptr);
sum+=(injector.create<X122*>() != nullptr);
sum+=(injector.create<X123*>() != nullptr);
sum+=(injector.create<X124*>() != nullptr);
sum+=(injector.create<X125*>() != nullptr);
sum+=(injector.create<X126*>() != nullptr);
sum+=(injector.create<X127*>() != nullptr);
sum+=(injector.create<X128*>() != nullptr);
sum+=(injector.create<X129*>() != nullptr);
sum+=(injector.create<X130*>() != nullptr);
sum+=(injector.create<X131*>() != nullptr);
sum+=(injector.create<X132*>() != nullptr);
sum+=(injector.create<X133*>() != nullptr);
sum+=(injector.create<X134*>() != nullptr);
sum+=(injector.create<X135*>() != nullptr);
sum+=(injector.create<X136*>() != nullptr);
sum+=(injector.create<X137*>() != nullptr);
sum+=(injector.create<X138*>() != nullptr);
sum+=(injector.create<X139*>() != nullptr);
sum+=(injector.create<X140*>() != nullptr);
sum+=(injector.create<X141*>() != nullptr);
sum+=(injector.create<X142*>() != nullptr);
sum+=(injector.create<X143*>() != nullptr);
sum+=(injector.create<X144*>() != nullptr);
sum+=(injector.create<X145*>() != nullptr);
sum+=(injector.create<X146*>() != nullptr);
sum+=(injector.create<X147*>() != nullptr);
sum+=(injector.create<X148*>() != nullptr);
sum+=(injector.create<X149*>() != nullptr);
sum+=(injector.create<X150*>() != nullptr);
sum+=(injector.create<X151*>() != nullptr);
sum+=(injector.create<X152*>() != nullptr);
sum+=(injector.create<X153*>() != nullptr);
sum+=(injector.create<X154*>() != nullptr);
sum+=(injector.create<X155*>() != nullptr);
sum+=(injector.create<X156*>() != nullptr);
sum+=(injector.create<X157*>() != nullptr);
sum+=(injector.create<X158*>() != nullptr);
sum+=(injector.create<X159*>() != nullptr);
sum+=(injector.create<X160*>() != nullptr);
sum+=(injector.create<X161*>() != nullptr);
sum+=(injector.create<X162*>() != nullptr);
sum+=(injector.create<X163*>() != nullptr);
sum+=(injector.create<X164*>() != nullptr);
sum+=(injector.create<X165*>() != nullptr);
sum+=(injector.create<X166*>() != nullptr);
sum+=(injector.create<X167*>() != nullptr);
sum+=(injector.create<X168*>() != nullptr);
sum+=(injector.create<X169*>() != nullptr);
sum+=(injector.create<X170*>() != nullptr);
sum+=(injector.create<X171*>() != nullptr);
sum+=(injector.create<X172*>() != nullptr);
sum+=(injector.create<X173*>() != nullptr);
sum+=(injector.create<X174*>() != nullptr);
sum+=(injector.create<X175*>() != nullptr);
sum+=(injector.create<X176*>() != nullptr);
sum+=(injector.create<X177*>() != nullptr);
sum+=(injector.create<X178*>() != nullptr);
sum+=(injector.create<X179*>() != nullptr);
sum+=(injector.create<X180*>() != nullptr);
sum+=(injector.create<X181*>() != nullptr);
sum+=(injector.create<X182*>() != nullptr);
sum+=(injector.create<X183*>() != nullptr);
sum+=(injector.create<X184*>() != nullptr);
sum+=(injector.create<X185*>() != nullptr);
sum+=(injector.create<X186*>() != nullptr);
sum+=(injector.create<X187*>() != nullptr);
sum+=(injector.create<X188*>() != nullptr);
sum+=(injector.create<X189*>() != nullptr);
sum+=(injector.create<X190*>() != nullptr);
sum+=(injector.create<X191*>() != nullptr);
sum+=(injector.create<X192*>() != nullptr);
sum+=(injector.create<X193*>() != nullptr);
sum+=(injector.create<X194*>() != nullptr);
sum+=(injector.create<X195*>() != nullptr);
sum+=(injector.create<X196*>() != nullptr);
sum+=(injector.create<X197*>() != nullptr);
sum+=(injector.create<X198*>() != nullptr);
sum+=(injector.create<X199*>() != nullptr);
#else
sum+=(factory::createX0() != nullptr);
sum+=(factory::createX1() != nullptr);
sum+=(factory::createX2() != nullptr);
sum+=(factory::createX3() != nullptr);
sum+=(factory::createX4() != nullptr);
sum+=(factory::createX5() != nullptr);
sum+=(factory::createX6() != nullptr);
sum+=(factory::createX7() != nullptr);
sum+=(factory::createX8() != nullptr);
sum+=(factory::createX9() != nullptr);
sum+=(factory::createX10() != nullptr);
sum+=(factory::createX11() != nullptr);
sum+=(factory::createX12() != nullptr);
sum+=(factory::createX13() != nullptr);
sum+=(factory::createX14() != nullptr);
sum+=(factory::createX15() != nullptr);
sum+=(factory::createX16() != nullptr);
sum+=(factory::createX17() != nullptr);
sum+=(factory::createX18() != nullptr);
sum+=(factory::createX19() != nullptr);
sum+=(factory::createX20() != nullptr);
sum+=(factory::createX21() != nullptr);
sum+=(factory::createX22() != nullptr);
sum+=(factory::createX23() != nullptr);
sum+=(factory::createX24() != nullptr);
sum+=(factory::createX25() != nullptr);
sum+=(factory::createX26() != nullptr);
sum+=(factory::createX27() != nullptr);
sum+=(factory::createX28() != nullptr);
sum+=(factory::createX29() != nullptr);
sum+=(factory::createX30() != nullptr);
sum+=(factory::createX31() != nullptr);
sum+=(factory::createX32() != nullptr);
sum+=(factory::createX33() != nullptr);
sum+=(factory::createX34() != nullptr);
sum+=(factory::createX35() != nullptr);
sum+=(factory::createX36() != nullptr);
sum+=(factory::createX37() != nullptr);
sum+=(factory::createX38() != nullptr);
sum+=(factory::createX39() != nullptr);
sum+=(factory::createX40() != nullptr);
sum+=(factory::createX41() != nullptr);
sum+=(factory::createX42() != nullptr);
sum+=(factory::createX43() != nullptr);
sum+=(factory::createX44() != nullptr);
sum+=(factory::createX45() != nullptr);
sum+=(factory::createX46() != nullptr);
sum+=(factory::createX47() != nullptr);
sum+=(factory::createX48() != nullptr);
sum+=(factory::createX49() != nullptr);
sum+=(factory::createX50() != nullptr);
sum+=(factory::createX51() != nullptr);
sum+=(factory::createX52() != nullptr);
sum+=(factory::createX53() != nullptr);
sum+=(factory::createX54() != nullptr);
sum+=(factory::createX55() != nullptr);
sum+=(factory::createX56() != nullptr);
sum+=(factory::createX57() != nullptr);
sum+=(factory::createX58() != nullptr);
sum+=(factory::createX59() != nullptr);
sum+=(factory::createX60() != nullptr);
sum+=(factory::createX61() != nullptr);
sum+=(factory::createX62() != nullptr);
sum+=(factory::createX63() != nullptr);
sum+=(factory::createX64() != nullptr);
sum+=(factory::createX65() != nullptr);
sum+=(factory::createX66() != nullptr);
sum+=(factory::createX67() != nullptr);
sum+=(factory::createX68() != nullptr);
sum+=(factory::createX69() != nullptr);
sum+=(factory::createX70() != nullptr);
sum+=(factory::createX71() != nullptr);
sum+=(factory::createX72() != nullptr);
sum+=(factory::createX73() != nullptr);
sum+=(factory::createX74() != nullptr);
sum+=(factory::createX75() != nullptr);
sum+=(factory::createX76() != nullptr);
sum+=(factory::createX77() != nullptr);
sum+=(factory::createX78() != nullptr);
sum+=(factory::createX79() != nullptr);
sum+=(factory::createX80() != nullptr);
sum+=(factory::createX81() != nullptr);
sum+=(factory::createX82() != nullptr);
sum+=(factory::createX83() != nullptr);
sum+=(factory::createX84() != nullptr);
sum+=(factory::createX85() != nullptr);
sum+=(factory::createX86() != nullptr);
sum+=(factory::createX87() != nullptr);
sum+=(factory::createX88() != nullptr);
sum+=(factory::createX89() != nullptr);
sum+=(factory::createX90() != nullptr);
sum+=(factory::createX91() != nullptr);
sum+=(factory::createX92() != nullptr);
sum+=(factory::createX93() != nullptr);
sum+=(factory::createX94() != nullptr);
sum+=(factory::createX95() != nullptr);
sum+=(factory::createX96() != nullptr);
sum+=(factory::createX97() != nullptr);
sum+=(factory::createX98() != nullptr);
sum+=(factory::createX99() != nullptr);
sum+=(factory::createX100() != nullptr);
//sum+=(factory::createX101() != nullptr);
//sum+=(factory::createX102() != nullptr);
//sum+=(factory::createX103() != nullptr);
//sum+=(factory::createX104() != nullptr);
//sum+=(factory::createX105() != nullptr);
//sum+=(factory::createX106() != nullptr);
//sum+=(factory::createX107() != nullptr);
//sum+=(factory::createX108() != nullptr);
//sum+=(factory::createX109() != nullptr);
//sum+=(factory::createX110() != nullptr);
//sum+=(factory::createX111() != nullptr);
//sum+=(factory::createX112() != nullptr);
//sum+=(factory::createX113() != nullptr);
//sum+=(factory::createX114() != nullptr);
//sum+=(factory::createX115() != nullptr);
//sum+=(factory::createX116() != nullptr);
//sum+=(factory::createX117() != nullptr);
//sum+=(factory::createX118() != nullptr);
//sum+=(factory::createX119() != nullptr);
//sum+=(factory::createX120() != nullptr);
//sum+=(factory::createX121() != nullptr);
//sum+=(factory::createX122() != nullptr);
//sum+=(factory::createX123() != nullptr);
//sum+=(factory::createX124() != nullptr);
//sum+=(factory::createX125() != nullptr);
//sum+=(factory::createX126() != nullptr);
//sum+=(factory::createX127() != nullptr);
//sum+=(factory::createX128() != nullptr);
//sum+=(factory::createX129() != nullptr);
//sum+=(factory::createX130() != nullptr);
//sum+=(factory::createX131() != nullptr);
//sum+=(factory::createX132() != nullptr);
//sum+=(factory::createX133() != nullptr);
//sum+=(factory::createX134() != nullptr);
//sum+=(factory::createX135() != nullptr);
//sum+=(factory::createX136() != nullptr);
//sum+=(factory::createX137() != nullptr);
//sum+=(factory::createX138() != nullptr);
//sum+=(factory::createX139() != nullptr);
//sum+=(factory::createX140() != nullptr);
//sum+=(factory::createX141() != nullptr);
//sum+=(factory::createX142() != nullptr);
//sum+=(factory::createX143() != nullptr);
//sum+=(factory::createX144() != nullptr);
//sum+=(factory::createX145() != nullptr);
//sum+=(factory::createX146() != nullptr);
//sum+=(factory::createX147() != nullptr);
//sum+=(factory::createX148() != nullptr);
//sum+=(factory::createX149() != nullptr);
//sum+=(factory::createX150() != nullptr);
//sum+=(factory::createX151() != nullptr);
//sum+=(factory::createX152() != nullptr);
//sum+=(factory::createX153() != nullptr);
//sum+=(factory::createX154() != nullptr);
//sum+=(factory::createX155() != nullptr);
//sum+=(factory::createX156() != nullptr);
//sum+=(factory::createX157() != nullptr);
//sum+=(factory::createX158() != nullptr);
//sum+=(factory::createX159() != nullptr);
//sum+=(factory::createX160() != nullptr);
//sum+=(factory::createX161() != nullptr);
//sum+=(factory::createX162() != nullptr);
//sum+=(factory::createX163() != nullptr);
//sum+=(factory::createX164() != nullptr);
//sum+=(factory::createX165() != nullptr);
//sum+=(factory::createX166() != nullptr);
//sum+=(factory::createX167() != nullptr);
//sum+=(factory::createX168() != nullptr);
//sum+=(factory::createX169() != nullptr);
//sum+=(factory::createX170() != nullptr);
//sum+=(factory::createX171() != nullptr);
//sum+=(factory::createX172() != nullptr);
//sum+=(factory::createX173() != nullptr);
//sum+=(factory::createX174() != nullptr);
//sum+=(factory::createX175() != nullptr);
//sum+=(factory::createX176() != nullptr);
//sum+=(factory::createX177() != nullptr);
//sum+=(factory::createX178() != nullptr);
//sum+=(factory::createX179() != nullptr);
//sum+=(factory::createX180() != nullptr);
//sum+=(factory::createX181() != nullptr);
//sum+=(factory::createX182() != nullptr);
//sum+=(factory::createX183() != nullptr);
//sum+=(factory::createX184() != nullptr);
//sum+=(factory::createX185() != nullptr);
//sum+=(factory::createX186() != nullptr);
//sum+=(factory::createX187() != nullptr);
//sum+=(factory::createX188() != nullptr);
//sum+=(factory::createX189() != nullptr);
//sum+=(factory::createX190() != nullptr);
//sum+=(factory::createX191() != nullptr);
//sum+=(factory::createX192() != nullptr);
//sum+=(factory::createX193() != nullptr);
//sum+=(factory::createX194() != nullptr);
//sum+=(factory::createX195() != nullptr);
//sum+=(factory::createX196() != nullptr);
//sum+=(factory::createX197() != nullptr);
//sum+=(factory::createX198() != nullptr);
//sum+=(factory::createX199() != nullptr);
#endif
}
return sum;
}


