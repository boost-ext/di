#include <di/registry.hpp>
#include <di/injector.hpp>

void module(di::registry& r) {
    r.add(r.type<int>().instance(new int{42}));
}

int main() {
    di::injector injector;
    injector.install(module);
    return injector.construct<int>() != 42;
}

