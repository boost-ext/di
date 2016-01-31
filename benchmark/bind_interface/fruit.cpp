#include <fruit/fruit.h>

struct I { virtual ~I() noexcept = default; virtual void dummy() = 0; }; struct Impl : I { INJECT(Impl()) { } void dummy() override { } };

fruit::Component<I> module() {
    return fruit::createComponent()
        .bind<I, Impl>();
}

int main() {
    fruit::Injector<I> injector{module()};
    std::unique_ptr<I> object{injector.get<I*>()};
    object->dummy();
}

