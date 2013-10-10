//
// Copyright (c) 2012-2013 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/injector.hpp"

#include <boost/test/unit_test.hpp>

namespace boost {
namespace di {

struct limit
{
    BOOST_DI_CTOR(limit, (
          int /*1*/
        , int /*2*/
        , int /*3*/
        , int /*4*/
        , int /*5*/
        , int /*6*/
        , int /*7*/
        , int /*8*/
        , int /*9*/
        , int /*10*/
        //, int [>11<]
        //, int [>12<]
        //, int [>13<]
        //, int [>14<]
        //, int [>15<]
        //, int [>16<]
        //, int [>17<]
        //, int [>18<]
        //, int [>19<]
        //, int [>20<]
        //, int [>21<]
        //, int [>22<]
        //, int [>23<]
        //, int [>24<]
        //, int [>25<]
        //, int [>26<]
        //, int [>27<]
        //, int [>28<]
        //, int [>29<]
        /*, int [>30<]*/
    )) { }
};

BOOST_AUTO_TEST_CASE(args_arity_limit_size) {
    injector<>().create<limit>();
}

} // namespace di
} // namespace boost

