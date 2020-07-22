//
// Copyright (c) 2012-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
using Ninject;

interface I {
  void dummy();
};

class Impl : I {
  public Impl() { }
  void I.dummy() { }
}

class Module : Ninject.Modules.NinjectModule {
  public override void Load() {
    //Bind<I>().To<Impl>();
  }
}

class ninject {
  static void Main(string[] args) {
    Ninject.IKernel kernel = new StandardKernel(new Module());
    I object = kernel.Get<I>();
    object->dummy();
  }
}

/**
 */
