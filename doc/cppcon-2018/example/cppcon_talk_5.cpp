#include <boost/di.hpp>
#include <memory>
#include <vector>
#include <string>
#include <cassert>
#include <type_traits>

namespace di = boost::di;

class Eniac {
 public:
   constexpr auto capacity() const { return 42;}
   constexpr auto layout() { }
};

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

template<class T>
concept bool RoomLike = not boost::di::aux::is_complete<T>::value or requires(T room) {
  { room.capacity() } -> int
  { room.layout()   }
};

template<RoomLike TRoom = class Room>
class cppcon_talk {
 public:
  cppcon_talk(TRoom&& room, std::unique_ptr<ispeaker> speaker, attendees&) {
    assert(dynamic_cast<regular_speaker*>(speaker.get()));
    static_assert(std::is_same_v<TRoom, Eniac>);
  }
};

int main() {
  const auto injector = di::make_injector(
    di::bind<name[]>.to({name{"Lenny"},
                         name{"Shea"},
                         name{"Daniel"}}),
    di::bind<ispeaker>.to<regular_speaker>(),
    di::bind<class Room>.to<Eniac>()
  );

  injector.create<cppcon_talk>();
}
