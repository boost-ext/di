#include <boost/di.hpp>
#include <memory>
#include <vector>
#include <string>
#include <cassert>

namespace di = boost::di;

class room {}; class speaker {};

struct name : std::string {
  using std::string::string;
};

class attendees {
 public:
   explicit attendees(const std::vector<name>& names) {
     assert(3u == names.size());
     assert("Lenny" == names[0]);
     assert("Shea" == names[1]);
     assert("Daniel" == names[2]);
   }
};

class cppcon_talk {
 public:
  cppcon_talk(room&&, std::unique_ptr<speaker>, attendees&) {}
};

int main() {
  const auto injector = di::make_injector(
    di::bind<name[]>.to({name{"Lenny"},
                         name{"Shea"},
                         name{"Daniel"}})
  );
  injector.create<cppcon_talk>();
}
