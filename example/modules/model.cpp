#include <cassert>
#include "model.hpp"

model::model(di::named<int, rows> r, di::named<int, colls> c, std::unique_ptr<iboard> b) {
    auto rows = r;
    auto colls = c;
    assert(rows == 42);
    assert(colls == 42);
    assert(b.get());
}

