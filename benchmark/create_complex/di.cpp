#include <boost/di.hpp>

namespace di = boost::di;

auto module = [] {
  auto injector =
      di::make_injector(di::bind<I00>.to<Impl00>(), di::bind<I01>.to<Impl01>(),
                        di::bind<I02>.to<Impl02>(), di::bind<I03>.to<Impl03>(),
                        di::bind<I04>.to<Impl04>(), di::bind<I05>.to<Impl05>(),
                        di::bind<I06>.to<Impl06>(), di::bind<I07>.to<Impl07>(),
                        di::bind<I08>.to<Impl08>(), di::bind<I09>.to<Impl09>(),
                        di::bind<I10>.to<Impl10>(), di::bind<I11>.to<Impl11>(),
                        di::bind<I12>.to<Impl12>(), di::bind<I13>.to<Impl13>(),
                        di::bind<I14>.to<Impl14>(), di::bind<I15>.to<Impl15>(),
                        di::bind<I16>.to<Impl16>(), di::bind<I17>.to<Impl17>(),
                        di::bind<I18>.to<Impl18>(), di::bind<I19>.to<Impl19>(),
                        di::bind<I20>.to<Impl20>(), di::bind<I21>.to<Impl21>(),
                        di::bind<I22>.to<Impl22>(), di::bind<I23>.to<Impl23>(),
                        di::bind<I24>.to<Impl24>(), di::bind<I25>.to<Impl25>(),
                        di::bind<I26>.to<Impl26>(), di::bind<I27>.to<Impl27>(),
                        di::bind<I28>.to<Impl28>(), di::bind<I29>.to<Impl29>(),
                        di::bind<I30>.to<Impl30>(), di::bind<I31>.to<Impl31>(),
                        di::bind<I32>.to<Impl32>(), di::bind<I33>.to<Impl33>(),
                        di::bind<I34>.to<Impl34>(), di::bind<I35>.to<Impl35>(),
                        di::bind<I36>.to<Impl36>(), di::bind<I37>.to<Impl37>(),
                        di::bind<I38>.to<Impl38>(), di::bind<I39>.to<Impl39>(),
                        di::bind<I40>.to<Impl40>(), di::bind<I41>.to<Impl41>(),
                        di::bind<I42>.to<Impl42>(), di::bind<I43>.to<Impl43>(),
                        di::bind<I44>.to<Impl44>(), di::bind<I45>.to<Impl45>(),
                        di::bind<I46>.to<Impl46>(), di::bind<I47>.to<Impl47>(),
                        di::bind<I48>.to<Impl48>(), di::bind<I49>.to<Impl49>(),
                        di::bind<I50>.to<Impl50>(), di::bind<I51>.to<Impl51>(),
                        di::bind<I52>.to<Impl52>(), di::bind<I53>.to<Impl53>(),
                        di::bind<I54>.to<Impl54>(), di::bind<I55>.to<Impl55>(),
                        di::bind<I56>.to<Impl56>(), di::bind<I57>.to<Impl57>(),
                        di::bind<I58>.to<Impl58>(), di::bind<I59>.to<Impl59>(),
                        di::bind<I60>.to<Impl60>(), di::bind<I61>.to<Impl61>(),
                        di::bind<I62>.to<Impl62>(), di::bind<I63>.to<Impl63>(),
                        di::bind<I64>.to<Impl64>(), di::bind<I65>.to<Impl65>(),
                        di::bind<I66>.to<Impl66>(), di::bind<I67>.to<Impl67>(),
                        di::bind<I68>.to<Impl68>(), di::bind<I69>.to<Impl69>(),
                        di::bind<I70>.to<Impl70>(), di::bind<I71>.to<Impl71>(),
                        di::bind<I72>.to<Impl72>(), di::bind<I73>.to<Impl73>(),
                        di::bind<I74>.to<Impl74>(), di::bind<I75>.to<Impl75>(),
                        di::bind<I76>.to<Impl76>(), di::bind<I77>.to<Impl77>(),
                        di::bind<I78>.to<Impl78>(), di::bind<I79>.to<Impl79>(),
                        di::bind<I80>.to<Impl80>(), di::bind<I81>.to<Impl81>(),
                        di::bind<I82>.to<Impl82>(), di::bind<I83>.to<Impl83>(),
                        di::bind<I84>.to<Impl84>(), di::bind<I85>.to<Impl85>(),
                        di::bind<I86>.to<Impl86>(), di::bind<I87>.to<Impl87>(),
                        di::bind<I88>.to<Impl88>(), di::bind<I89>.to<Impl89>(),
                        di::bind<I90>.to<Impl90>(), di::bind<I91>.to<Impl91>(),
                        di::bind<I92>.to<Impl92>(), di::bind<I93>.to<Impl93>(),
                        di::bind<I94>.to<Impl94>(), di::bind<I95>.to<Impl95>(),
                        di::bind<I96>.to<Impl96>(), di::bind<I97>.to<Impl97>(),
                        di::bind<I98>.to<Impl98>(), di::bind<I99>.to<Impl99>());

  // using injector_t = decltype(injector);
  // struct : injector_t {
  //   using injector::injector;
  // } i{static_cast<injector_t &&>(injector)};
  // return i;

  return injector;
};

int main() {
  auto injector = di::make_injector(module());
  injector.create<BigComplexity>();
}
