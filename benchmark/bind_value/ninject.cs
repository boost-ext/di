//
// Copyright (c) 2012-2019 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
using Ninject;

class Module : Ninject.Modules.NinjectModule {
  public override void Load() {
    Bind<int>().To(42);
  }
}

class ninject {
  static void Main(string[] args) {
    Ninject.IKernel kernel = new StandardKernel(new Module());
    kernel.Get<int>();
  }
}
