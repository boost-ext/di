#include <cassert>
#include <boost/di.hpp>
#include "model.hpp"

model::model(int rows, int colls, std::unique_ptr<iboard> b) {
    assert(rows == 42);
    assert(colls == 42);
    assert(b.get());
}

