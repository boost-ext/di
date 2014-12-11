#include <boost/di.hpp>
#include "module_view.hpp"
#include "module_model.hpp"
#include "controller.hpp"
#include "config.hpp"

namespace di = boost::di;

int main() {
   auto injector = di::make_injector(
        module_view{}
      , module_model{}
    );

    return injector.create<controller>().run();
}

