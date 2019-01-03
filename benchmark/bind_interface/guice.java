//
// Copyright (c) 2012-2019 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
import com.google.inject.*;

interface I {
  public void dummy();
};

class Impl implements I {
  @Inject Impl() { }
  @Override public void dummy() { }
}

class Module extends AbstractModule {
  @Override
  protected void configure() {
    bind(I.class).to(Impl.class);
  }
}

public class guice {
  public static void main(String[] args) {
    Injector injector = Guice.createInjector(new Module());
    I object = injector.getInstance(I.class);
  }
}
