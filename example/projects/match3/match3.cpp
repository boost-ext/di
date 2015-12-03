// clang-format off
// clang++ match3.cpp -I ../../../include -I../msm -ISDL2 -std=c++1y  -lSDL2 -lSDL2_image -L. && LD_LIBRARY_PATH=. ./a.out
// emcc match3.cpp -I . -I SDL2 -std=c++1y -s USE_SDL=2 -s USE_SDL_IMAGE=2 -o match3.html
// clang-format on

#if __has_include(<SDL.h>)&&__has_include(<SDL_image.h>)
#include <SDL.h>
#include <SDL_image.h>
#endif
#include <iostream>
#include <memory>
#include <cassert>
#include <string>
#include <array>
#include <map>
#include <boost/di.hpp>
#include "msm.hpp"

class SDL_Texture;

namespace match3 {

class icanvas {
 public:
  virtual ~icanvas() noexcept = default;
  virtual std::shared_ptr<SDL_Texture> load_image(const std::string&) const = 0;
  virtual void draw(std::shared_ptr<SDL_Texture>, int x = 0, int y = 0, int layer = 0) = 0;
  virtual void render() = 0;
  virtual void clear() = 0;
};

class iclient {
 public:
  virtual ~iclient() noexcept = default;
  virtual void run() = 0;
};

using idispatcher = msm::idispatcher<SDL_Event>;

#if __has_include(<SDL.h>)&&__has_include(<SDL_image.h>)
class sdl_canvas : public icanvas {
  const int RENDER_DRIVER = -1;
  const int RENDER_FLAGS = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;

 public:
  sdl_canvas() noexcept {
    assert(!SDL_Init(SDL_INIT_EVERYTHING & ~(SDL_INIT_TIMER | SDL_INIT_HAPTIC)) && SDL_GetError());

    window_ = std::shared_ptr<SDL_Window>(
        SDL_CreateWindow("", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN),
        SDL_DestroyWindow);

    renderer_ = std::shared_ptr<SDL_Renderer>(SDL_CreateRenderer(window_.get(), RENDER_DRIVER, RENDER_FLAGS),
                                              SDL_DestroyRenderer);

    SDL_SetRenderDrawBlendMode(renderer_.get(), SDL_BLENDMODE_BLEND);
  }

  ~sdl_canvas() noexcept override { SDL_Quit(); }

  void draw(std::shared_ptr<SDL_Texture> texture, int x, int y, int layer) override {
    SDL_Rect pos = {x, y, 0, 0};
    SDL_QueryTexture(texture.get(), nullptr, nullptr, &pos.w, &pos.h);
    layers_[layer].push_back(std::make_pair(texture, pos));
  }

  std::shared_ptr<SDL_Texture> load_image(const std::string& file) const override {
    return std::shared_ptr<SDL_Texture>(IMG_LoadTexture(renderer_.get(), file.c_str()), SDL_DestroyTexture);
  }

  void render() override {
    for (const auto& layer : layers_) {
      for (const auto& data : layer.second) {
        SDL_RenderCopy(renderer_.get(), data.first.get(), nullptr, &data.second);
      }
    }

    SDL_RenderPresent(renderer_.get());
  }

  void clear() override {
    layers_.clear();
    SDL_RenderClear(renderer_.get());
  }

 private:
  std::shared_ptr<SDL_Window> window_;
  std::shared_ptr<SDL_Renderer> renderer_;
  std::map<int, std::vector<std::pair<std::shared_ptr<SDL_Texture>, SDL_Rect>>> layers_;
};
#endif

class view {
 public:
  view(icanvas& c) : canvas_(c) {
    canvas_.draw(canvas_.load_image("background.png"), 0, 0, 0);

    for (int i = 1; i <= 5; ++i) {
      grids[i - 1] = canvas_.load_image(std::to_string(i) + ".png");
    };

    canvas_.render();
  }

  void show_grid(int x, int y, int c) {
    canvas_.draw(grids[c], x * 38 + y * 38, 1);
    canvas_.render();
  }

  std::array<std::shared_ptr<SDL_Texture>, 5> grids;
  icanvas& canvas_;
};

class board {};

struct is_game_over {};

struct button_clicked {
  explicit button_clicked(const SDL_Event& e) : x(e.button.x), y(e.button.y) {}
  static constexpr auto id = SDL_MOUSEBUTTONUP;
  int x = 0;
  int y = 0;
};

struct key_pressed {
  static constexpr auto id = SDL_KEYDOWN;
  explicit key_pressed(const SDL_Event& event) : key(event.key.keysym.sym) {}
  int key = 0;
};

struct window_closed {
  explicit window_closed(const SDL_Event&) {}
  static constexpr auto id = SDL_QUIT;
};

auto guard = [](auto) { return true; };
auto is_key = [](int key) { return [](auto e) { return true; }; };

auto init_board = [](auto, view& v) {
  for (int i = 0; i < 3; ++i) {
    v.show_grid(i, i, 2);
  }
};

auto print = [](auto) { std::cout << "clicked" << std::endl; };

auto controller__() noexcept {
  struct config {
    static void no_transition() noexcept {}
  };

  using namespace msm;
  auto idle = init_state<__COUNTER__>{};
  auto wait_for_client = init_state<__COUNTER__>{};
  auto game_over = state<__COUNTER__, is_game_over>{};
  auto s1 = state<__COUNTER__>{};
  auto s2 = state<__COUNTER__>{};

  // clang-format off
  return make_transition_table<config>(
   // +-----------------------------------------------------------------+
      idle    		   == idle / init_board
	, idle     		   == s1 + event<button_clicked> / print
   // +-----------------------------------------------------------------+
	, wait_for_client  == game_over + event<window_closed>
	, wait_for_client  == game_over + event<key_pressed> [is_key(SDLK_ESCAPE)] / [] (auto) {std::cout << "exit" << std::endl; }
   // +-----------------------------------------------------------------+
  );
  // clang-format on
}

using controller = decltype(controller__());

#if __has_include(<SDL.h>)&&__has_include(<SDL_image.h>)
struct sdl_user : iclient {
  explicit sdl_user(idispatcher& d, controller& c) : dispatcher_(d), controller_(c) {}

  void run() override {
    SDL_Event event = {};
    while (!controller_.is_in_state<is_game_over>()) {
      while (SDL_PollEvent(&event)) {
        dispatcher_.dispatch_event(event);
      }
    }
  }

 private:
  idispatcher& dispatcher_;
  controller& controller_;
};
#endif

class game {
 public:
  game(controller& c, iclient& cl) : controller_(c), client_(cl) {}

  void play() {
    controller_.start();
    client_.run();
  }

  iclient& client_;
  controller& controller_;
};

struct sdl_event_dispatcher {
  static auto get_id(SDL_Event event) noexcept { return event.type; }
  template <class T>
  static auto get_id() noexcept {
    return T::id;
  }
  template <class T>
  static auto get_event(SDL_Event event) noexcept {
    return T(event);
  }
};

class sdl_dispatcher : public idispatcher {
 public:
  sdl_dispatcher(controller& c) { dispatcher.register_handler(c); }
  void dispatch_event(SDL_Event event) noexcept override { dispatcher.dispatch_event(event); }

 private:
  msm::dispatcher<SDL_Event, sdl_event_dispatcher> dispatcher;
};

auto configuration = [] {
  return di::make_injector(di::bind<icanvas>.to<sdl_canvas>(), di::bind<iclient>.to<sdl_user>(),
                           di::bind<idispatcher>.to<sdl_dispatcher>());
};

}  // match3

int main() {
  auto injector = di::make_injector(match3::configuration());
  injector.create<match3::game>().play();
}
