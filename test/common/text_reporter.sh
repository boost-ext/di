#!/bin/bash
#
# Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
#
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#

if [[ $3 != "run" ]]; then
    test_case=`nm -C $1 | grep ${2:2} | cut -d ' ' -f3`
    echo "$3:$4:FAIL [$test_case] \"${@:6}\""
fi

