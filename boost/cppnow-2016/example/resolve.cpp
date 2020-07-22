//
// Copyright (c) 2012-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <type_traits>

template<class, class> struct pair {};

template<class T>
struct dependency_concept { };

template<class I, class Impl>
struct dependency : pair<dependency_concept<I>, dependency<I, Impl>> { };

template<class... Ts>
struct injector : Ts...  { };

template<class... Ts>
auto make_injector(Ts...) {
  return injector<Ts...>{};
}

struct binder {
  template <class TDefault, class>
  static TDefault resolve_impl(...) noexcept {
    return {};
  }
  template <class, class TConcept, class TDependency>
  static decltype(auto)
  resolve_impl(pair<TConcept, TDependency>* dep) noexcept {
    return static_cast<TDependency&>(*dep);
  }

  template <class T, class TDefault, class TDeps>
  static decltype(auto) resolve(TDeps* deps) noexcept {
    using dependency = dependency_concept<std::decay_t<T>>;
    return resolve_impl<TDefault, dependency>(deps);
  }
};

struct i0 {}; struct impl0 {};
struct i1 {}; struct impl1 {};
struct i2 {}; struct impl2 {};
struct i3 {}; struct impl3 {};
struct i4 {}; struct impl4 {};
struct i5 {}; struct impl5 {};
struct i6 {}; struct impl6 {};
struct i7 {}; struct impl7 {};
struct i8 {}; struct impl8 {};
struct i9 {}; struct impl9 {};
struct i10 {}; struct impl10 {};
struct i11 {}; struct impl11 {};
struct i12 {}; struct impl12 {};
struct i13 {}; struct impl13 {};
struct i14 {}; struct impl14 {};
struct i15 {}; struct impl15 {};
struct i16 {}; struct impl16 {};
struct i17 {}; struct impl17 {};
struct i18 {}; struct impl18 {};
struct i19 {}; struct impl19 {};
struct i20 {}; struct impl20 {};
struct i21 {}; struct impl21 {};
struct i22 {}; struct impl22 {};
struct i23 {}; struct impl23 {};
struct i24 {}; struct impl24 {};
struct i25 {}; struct impl25 {};
struct i26 {}; struct impl26 {};
struct i27 {}; struct impl27 {};
struct i28 {}; struct impl28 {};
struct i29 {}; struct impl29 {};
struct i30 {}; struct impl30 {};
struct i31 {}; struct impl31 {};
struct i32 {}; struct impl32 {};
struct i33 {}; struct impl33 {};
struct i34 {}; struct impl34 {};
struct i35 {}; struct impl35 {};
struct i36 {}; struct impl36 {};
struct i37 {}; struct impl37 {};
struct i38 {}; struct impl38 {};
struct i39 {}; struct impl39 {};
struct i40 {}; struct impl40 {};
struct i41 {}; struct impl41 {};
struct i42 {}; struct impl42 {};
struct i43 {}; struct impl43 {};
struct i44 {}; struct impl44 {};
struct i45 {}; struct impl45 {};
struct i46 {}; struct impl46 {};
struct i47 {}; struct impl47 {};
struct i48 {}; struct impl48 {};
struct i49 {}; struct impl49 {};
struct i50 {}; struct impl50 {};
struct i51 {}; struct impl51 {};
struct i52 {}; struct impl52 {};
struct i53 {}; struct impl53 {};
struct i54 {}; struct impl54 {};
struct i55 {}; struct impl55 {};
struct i56 {}; struct impl56 {};
struct i57 {}; struct impl57 {};
struct i58 {}; struct impl58 {};
struct i59 {}; struct impl59 {};
struct i60 {}; struct impl60 {};
struct i61 {}; struct impl61 {};
struct i62 {}; struct impl62 {};
struct i63 {}; struct impl63 {};
struct i64 {}; struct impl64 {};
struct i65 {}; struct impl65 {};
struct i66 {}; struct impl66 {};
struct i67 {}; struct impl67 {};
struct i68 {}; struct impl68 {};
struct i69 {}; struct impl69 {};
struct i70 {}; struct impl70 {};
struct i71 {}; struct impl71 {};
struct i72 {}; struct impl72 {};
struct i73 {}; struct impl73 {};
struct i74 {}; struct impl74 {};
struct i75 {}; struct impl75 {};
struct i76 {}; struct impl76 {};
struct i77 {}; struct impl77 {};
struct i78 {}; struct impl78 {};
struct i79 {}; struct impl79 {};
struct i80 {}; struct impl80 {};
struct i81 {}; struct impl81 {};
struct i82 {}; struct impl82 {};
struct i83 {}; struct impl83 {};
struct i84 {}; struct impl84 {};
struct i85 {}; struct impl85 {};
struct i86 {}; struct impl86 {};
struct i87 {}; struct impl87 {};
struct i88 {}; struct impl88 {};
struct i89 {}; struct impl89 {};
struct i90 {}; struct impl90 {};
struct i91 {}; struct impl91 {};
struct i92 {}; struct impl92 {};
struct i93 {}; struct impl93 {};
struct i94 {}; struct impl94 {};
struct i95 {}; struct impl95 {};
struct i96 {}; struct impl96 {};
struct i97 {}; struct impl97 {};
struct i98 {}; struct impl98 {};
struct i99 {}; struct impl99 {};
struct i100 {}; struct impl100 {};
struct i101 {}; struct impl101 {};
struct i102 {}; struct impl102 {};
struct i103 {}; struct impl103 {};
struct i104 {}; struct impl104 {};
struct i105 {}; struct impl105 {};
struct i106 {}; struct impl106 {};
struct i107 {}; struct impl107 {};
struct i108 {}; struct impl108 {};
struct i109 {}; struct impl109 {};
struct i110 {}; struct impl110 {};
struct i111 {}; struct impl111 {};
struct i112 {}; struct impl112 {};
struct i113 {}; struct impl113 {};
struct i114 {}; struct impl114 {};
struct i115 {}; struct impl115 {};
struct i116 {}; struct impl116 {};
struct i117 {}; struct impl117 {};
struct i118 {}; struct impl118 {};
struct i119 {}; struct impl119 {};
struct i120 {}; struct impl120 {};
struct i121 {}; struct impl121 {};
struct i122 {}; struct impl122 {};
struct i123 {}; struct impl123 {};
struct i124 {}; struct impl124 {};
struct i125 {}; struct impl125 {};
struct i126 {}; struct impl126 {};
struct i127 {}; struct impl127 {};
struct i128 {}; struct impl128 {};

int main() {
  auto injector = make_injector(
    dependency<i1, impl1>{}
  , dependency<i2, impl2>{}
  , dependency<i3, impl3>{}
  , dependency<i4, impl4>{}
  , dependency<i5, impl5>{}
  , dependency<i6, impl6>{}
  , dependency<i7, impl7>{}
  , dependency<i8, impl8>{}
  , dependency<i9, impl9>{}
  , dependency<i10, impl10>{}
  , dependency<i11, impl11>{}
  , dependency<i12, impl12>{}
  , dependency<i13, impl13>{}
  , dependency<i14, impl14>{}
  , dependency<i15, impl15>{}
  , dependency<i16, impl16>{}
  , dependency<i17, impl17>{}
  , dependency<i18, impl18>{}
  , dependency<i19, impl19>{}
  , dependency<i20, impl20>{}
  , dependency<i21, impl21>{}
  , dependency<i22, impl22>{}
  , dependency<i23, impl23>{}
  , dependency<i24, impl24>{}
  , dependency<i25, impl25>{}
  , dependency<i26, impl26>{}
  , dependency<i27, impl27>{}
  , dependency<i28, impl28>{}
  , dependency<i29, impl29>{}
  , dependency<i30, impl30>{}
  , dependency<i31, impl31>{}
  , dependency<i32, impl32>{}
  , dependency<i33, impl33>{}
  , dependency<i34, impl34>{}
  , dependency<i35, impl35>{}
  , dependency<i36, impl36>{}
  , dependency<i37, impl37>{}
  , dependency<i38, impl38>{}
  , dependency<i39, impl39>{}
  , dependency<i40, impl40>{}
  , dependency<i41, impl41>{}
  , dependency<i42, impl42>{}
  , dependency<i43, impl43>{}
  , dependency<i44, impl44>{}
  , dependency<i45, impl45>{}
  , dependency<i46, impl46>{}
  , dependency<i47, impl47>{}
  , dependency<i48, impl48>{}
  , dependency<i49, impl49>{}
  , dependency<i50, impl50>{}
  , dependency<i51, impl51>{}
  , dependency<i52, impl52>{}
  , dependency<i53, impl53>{}
  , dependency<i54, impl54>{}
  , dependency<i55, impl55>{}
  , dependency<i56, impl56>{}
  , dependency<i57, impl57>{}
  , dependency<i58, impl58>{}
  , dependency<i59, impl59>{}
  , dependency<i60, impl60>{}
  , dependency<i61, impl61>{}
  , dependency<i62, impl62>{}
  , dependency<i63, impl63>{}
  , dependency<i64, impl64>{}
  , dependency<i65, impl65>{}
  , dependency<i66, impl66>{}
  , dependency<i67, impl67>{}
  , dependency<i68, impl68>{}
  , dependency<i69, impl69>{}
  , dependency<i70, impl70>{}
  , dependency<i71, impl71>{}
  , dependency<i72, impl72>{}
  , dependency<i73, impl73>{}
  , dependency<i74, impl74>{}
  , dependency<i75, impl75>{}
  , dependency<i76, impl76>{}
  , dependency<i77, impl77>{}
  , dependency<i78, impl78>{}
  , dependency<i79, impl79>{}
  , dependency<i80, impl80>{}
  , dependency<i81, impl81>{}
  , dependency<i82, impl82>{}
  , dependency<i83, impl83>{}
  , dependency<i84, impl84>{}
  , dependency<i85, impl85>{}
  , dependency<i86, impl86>{}
  , dependency<i87, impl87>{}
  , dependency<i88, impl88>{}
  , dependency<i89, impl89>{}
  , dependency<i90, impl90>{}
  , dependency<i91, impl91>{}
  , dependency<i92, impl92>{}
  , dependency<i93, impl93>{}
  , dependency<i94, impl94>{}
  , dependency<i95, impl95>{}
  , dependency<i96, impl96>{}
  , dependency<i97, impl97>{}
  , dependency<i98, impl98>{}
  , dependency<i99, impl99>{}
  , dependency<i100, impl100>{}
  , dependency<i101, impl101>{}
  , dependency<i102, impl102>{}
  , dependency<i103, impl103>{}
  , dependency<i104, impl104>{}
  , dependency<i105, impl105>{}
  , dependency<i106, impl106>{}
  , dependency<i107, impl107>{}
  , dependency<i108, impl108>{}
  , dependency<i109, impl109>{}
  , dependency<i110, impl110>{}
  , dependency<i111, impl111>{}
  , dependency<i112, impl112>{}
  , dependency<i113, impl113>{}
  , dependency<i114, impl114>{}
  , dependency<i115, impl115>{}
  , dependency<i116, impl116>{}
  , dependency<i117, impl117>{}
  , dependency<i118, impl118>{}
  , dependency<i119, impl119>{}
  , dependency<i120, impl120>{}
  , dependency<i121, impl121>{}
  , dependency<i122, impl122>{}
  , dependency<i123, impl123>{}
  , dependency<i124, impl124>{}
  , dependency<i125, impl125>{}
  , dependency<i126, impl126>{}
  , dependency<i127, impl127>{}
  , dependency<i128, impl128>{}
  );

  struct default_dependency{};
  static_assert(std::is_same<dependency<i64, impl64>, std::decay_t<decltype(binder{}.resolve<i64, default_dependency>(&injector))>>{});
  static_assert(std::is_same<default_dependency, std::decay_t<decltype(binder{}.resolve<i0, default_dependency>(&injector))>>{});
}
