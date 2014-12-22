#include <boost/di.hpp>
#include "module_model.hpp"
#include "model.hpp"
#include "common.hpp"
#include "config.hpp"

namespace di = boost::di;

di::injector<model> module_model::configure() const noexcept {
    return di::make_injector(
        di::bind<iboard, board>
      , di::bind<int>.named(rows).to(42)
      , di::bind<int>.named(colls).to(42)
    );
}

