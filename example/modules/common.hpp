//
// Copyright (c) 2012-2019 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//<-
#ifndef COMMON_HPP
#define COMMON_HPP
//->

struct ibutton {
  virtual ~ibutton() noexcept = default;
  virtual void dummy() = 0;
};
struct button : ibutton {
  void dummy() override{};
};
struct iwindow {
  virtual ~iwindow() noexcept = default;
  virtual void dummy() = 0;
};
struct window : iwindow {
  void dummy() override{};
};
struct iboard {
  virtual ~iboard() noexcept = default;
  virtual void dummy() = 0;
};
struct board : iboard {
  void dummy() override{};
};

//<-
#endif
//->
