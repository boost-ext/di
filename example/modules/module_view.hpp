#ifndef MODULE_VIEW_HPP
#define MODULE_VIEW_HPP
#include <boost/di.hpp>

class view;

//namespace boost { namespace di {
//template<class...>
//class injector;
//}}

namespace di = boost::di;

class module_view {
public:
    di::injector<view> configure() const noexcept;
};

#endif

