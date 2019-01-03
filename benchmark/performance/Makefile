#
# Copyright (c) 2012-2019 Kris Jusiak (kris at jusiak dot net)
#
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#
CXXFLAGS?="-O2 -std=c++1y -I ../../include"

all test:
	CXXFLAGS=$(CXXFLAGS) ./di.sh

small:
	COMPLEXITY=small CXXFLAGS=$(CXXFLAGS) ./di.sh

medium:
	COMPLEXITY=medium CXXFLAGS=$(CXXFLAGS) ./di.sh

big:
	COMPLEXITY=big CXXFLAGS=$(CXXFLAGS) ./di.sh

quick:
	COMPLEXITY=quick CXXFLAGS=$(CXXFLAGS) ./di.sh

header:
	$(CXX) $(CXXFLAGS) header.cpp

