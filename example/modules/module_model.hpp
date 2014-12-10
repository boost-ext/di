#ifndef MODULE_MODEL_HPP
#define MODULE_MODEL_HPP

#include <boost/di/fwd.hpp>

namespace di = boost::di;

class model;

class module_model {
public:
    di::injector<model> configure() const noexcept;
};

#endif

