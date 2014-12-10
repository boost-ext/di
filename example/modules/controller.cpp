#include <cassert>
#include "controller.hpp"

int controller::run() {
    assert(view_.get());
    assert(model_.get());

    return 0;
}

