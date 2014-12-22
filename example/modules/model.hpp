#ifndef MODEL_HPP
#define MODEL_HPP

#include <memory>
#include <boost/di/inject.hpp>
#include "common.hpp"

namespace di = boost::di;

static struct rows{} rows;
static struct colls{} colls;

class model {
public:
    BOOST_DI_INJECT(model, (named = rows) int, (named = colls) int, std::unique_ptr<iboard>);
};

#endif

