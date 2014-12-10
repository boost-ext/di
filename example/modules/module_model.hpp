#ifndef MODULE_MODEL_HPP
#define MODULE_MODEL_HPP

#include <boost/di.hpp>

class model;

namespace di = boost::di;

class module_model {
public:
    di::injector<model> configure() const noexcept;
};

#endif

