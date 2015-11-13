//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <memory>
#include <boost/di.hpp>

namespace di = boost::di;

struct iheater {
  virtual ~iheater() noexcept = default;
  virtual void on() = 0;
  virtual void off() = 0;
  virtual bool is_on() const = 0;
};

struct ipump {
  virtual ~ipump() noexcept = default;
  virtual void pump() = 0;
};

class heat_pump : public ipump {
 public:
  explicit heat_pump(std::shared_ptr<iheater> heater) : heater(heater) {}

  void pump() override {
    if (heater->is_on()) {
      //...
    }
  }

 private:
  std::shared_ptr<iheater> heater;
};

class electric_heater : public iheater {
 public:
  void on() override {}
  void off() override {}
  bool is_on() const override { return false; }
};

void no_di() {
  class coffee_maker {
    std::shared_ptr<iheater> heater = std::make_shared<electric_heater>();
    std::unique_ptr<ipump> pump = std::make_unique<heat_pump>(heater);

  public:
    void brew() {
      heater->on();
      pump->pump();
      heater->off();
    }
  };

  coffee_maker cm;
  cm.brew();
}

void manual_di() {
  class coffee_maker {
  public:
    coffee_maker(const std::shared_ptr<iheater>& heater, std::unique_ptr<ipump> pump)
        : heater(heater), pump(std::move(pump))
    { }

    void brew() {
      heater->on();
      pump->pump();
      heater->off();
    }

  private:
    std::shared_ptr<iheater> heater;
    std::unique_ptr<ipump> pump;
  };

  // has to be before pump
  auto heater = std::shared_ptr<iheater>{std::make_shared<electric_heater>()};

  // has to be after heater
  auto pump = std::unique_ptr<ipump>{std::make_unique<heat_pump>(heater)};

  coffee_maker cm{heater, std::move(pump)};
  cm.brew();
}

void automatic_di() {
  struct coffee_maker {
  public:
    coffee_maker(const std::shared_ptr<iheater>& heater, std::unique_ptr<ipump> pump)
        : heater(heater), pump(std::move(pump))
    { }

    void brew() {
      heater->on();
      pump->pump();
      heater->off();
    }

  private:
    std::shared_ptr<iheater> heater;
    std::unique_ptr<ipump> pump;
  };

  auto injector = di::make_injector(di::bind<ipump>().to<heat_pump>(), di::bind<iheater>().to<electric_heater>());

  auto cm = injector.create<coffee_maker>();
  cm.brew();
}

int main() {
  manual_di();
  automatic_di();
}
