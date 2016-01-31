#include <fruit/fruit.h>

auto i = 42; // bindInstance doesn't support l-value's

fruit::Component<int> module() {
    return fruit::createComponent()
        .bindInstance(i);
}

int main() {
    fruit::Injector<int> injector{module()};
    return injector.get<int>() != 42;
}

