#ifndef VIEW_HPP
#define VIEW_HPP

#include <memory>
#include "common.hpp"

class view {
public:
    view(std::unique_ptr<iwindow>, std::shared_ptr<ibutton>);
};

#endif

