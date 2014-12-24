#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <iostream>
#include <boost/di/config.hpp>

class config : public di::config {
public:
    auto policies() const noexcept {
        return di::make_policies(
            [](auto type){ std::cout << typeid(typename decltype(type)::arg::type).name() << std::endl; }
        );
    }
};

#endif

