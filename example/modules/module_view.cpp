#include <boost/di.hpp>
#include "module_view.hpp"
#include "common.hpp"
#include "view.hpp"
#include "config.hpp"

namespace di = boost::di;

di::injector<view> module_view::configure() const noexcept {
    return di::make_injector(
        di::bind<ibutton, button>
      , di::bind<iwindow, window>
    );
}

