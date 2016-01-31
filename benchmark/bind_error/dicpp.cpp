#include <di/registry.hpp>
#include <di/injector.hpp>
#include <di/constructor.hpp>

struct I { virtual ~I() noexcept = default; virtual void dummy() = 0; }; struct Impl : I { DI_CONSTRUCTOR(Impl, ()) { } void dummy() override { } };

void module(di::registry& r) {
    //r.add(r.type<I>().implementation<Impl>());
}

int main() {
    di::injector injector;
    injector.install(module);
    std::unique_ptr<I> object{injector.construct<I*>()};
    object->dummy();
}

//terminate called after throwing an instance of 'std::runtime_error'
  //what():  Requested type without corresponding provider: di::type_key<I*, void>. Double check if the type is abstract

