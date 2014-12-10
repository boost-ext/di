#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include <memory>
#include "model.hpp"
#include "view.hpp"

struct controller {
    std::shared_ptr<view> view_;
    std::shared_ptr<model> model_;

    int run();
};

#endif

