//
// Copyright (c) 2012-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
import com.google.inject.*;

class Module extends AbstractModule {
  @Override
  protected void configure() {
    bind(int.class).toInstance(42);
  }
}

public class guice {
  public static void main(String[] args) {
    Injector injector = Guice.createInjector(new Module());
    injector.getInstance(int.class);
  }
}
