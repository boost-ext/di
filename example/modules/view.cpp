#include <cassert>
#include "view.hpp"

view::view(std::unique_ptr<iwindow> w, std::shared_ptr<ibutton> b) {
    assert(w.get());
    assert(b.get());
}

