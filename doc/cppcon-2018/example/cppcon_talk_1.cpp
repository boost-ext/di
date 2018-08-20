#include <boost/di.hpp>
#include <memory>

namespace di = boost::di;

class room {}; class speaker {}; class attendees {};

class cppcon_talk {
 public:
  cppcon_talk(room&&, std::unique_ptr<speaker>, attendees&) {}
};

int main() {
  const auto injector = di::make_injector();
  injector.create<cppcon_talk>();
}
