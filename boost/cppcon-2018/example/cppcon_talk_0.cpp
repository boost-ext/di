#include <boost/di.hpp>

namespace di = boost::di;

class room {}; class speaker {}; class attendees {};

class cppcon_talk {
 public:
  cppcon_talk(const room&, const speaker&, attendees&) {}
};

int main() {
  const auto injector = di::make_injector();
  injector.create<cppcon_talk>();
}
