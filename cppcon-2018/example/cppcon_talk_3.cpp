#include <boost/di.hpp>
#include <memory>
#include <vector>
#include <string>
#include <cassert>

namespace di = boost::di;

class room {};

struct ispeaker {
  virtual ~ispeaker() noexcept = default;
  virtual void present() = 0;
};

struct keynote_speaker : ispeaker {
  void present() override { }
};

struct regular_speaker : ispeaker {
  void present() override { }
};

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
  cppcon_talk(room&&, std::unique_ptr<ispeaker> speaker, attendees&) {
    assert(dynamic_cast<regular_speaker*>(speaker.get()));
  }
};

int main() {
  const auto injector = di::make_injector(
    di::bind<name[]>.to({name{"Lenny"},
                         name{"Shea"},
                         name{"Daniel"}}),
    di::bind<ispeaker>.to<regular_speaker>()
  );

  injector.create<cppcon_talk>();
}
