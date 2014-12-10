#ifndef MODULE_VIEW_HPP
#define MODULE_VIEW_HPP

#include <boost/di/fwd.hpp>

namespace di = boost::di;

class view;

class module_view {
public:
    di::injector<view> configure() const noexcept;
};

#endif

