#ifndef MODEL_HPP
#define MODEL_HPP

#include <memory>
#include <boost/di/fwd.hpp>
#include "common.hpp"

namespace di = boost::di;

struct rows{};
struct colls{};

class model {
public:
    model(di::named<int, rows>, di::named<int, colls>, std::unique_ptr<iboard>);
};

#endif

