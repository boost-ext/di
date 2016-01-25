// clang-format off
// clang++ match3.cpp -I ../../../include -I../msm -ISDL2 -std=c++1y  -lSDL2 -lSDL2_image -L. && LD_LIBRARY_PATH=. ./a.out
// EMCC_DEBUG=1 em++ -Wwarn-absolute-paths -O2 match3.cpp -s SAFE_HEAP=1 -s ASSERTIONS=2 -I ../msm -I../../../include -std=c++1y -s USE_SDL=2 -s USE_SDL_IMAGE=2 -DDEBUG_IMGLIB -o match3.html --preload-file images --use-preload-plugins -Wwarn-absolute-paths && ~/Downloads/chrome/opt/google/chrome/chrome --disable-web-security match3.html
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
#include <random>
#include <boost/di.hpp>
#if defined(EMSCRIPTEN)
#include "emscripten.h"
#endif
#include "boost/msm/msm.hpp"

class SDL_Texture;

namespace di = boost::di;
namespace msm = boost::msm;

namespace match3 {

class icanvas {
 public:
  // TODO sprite
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
  static constexpr auto grid_size = 38;
  static constexpr auto grid_offset = grid_size + 5;
  static constexpr auto grids_offset_x = 328;
  static constexpr auto grids_offset_y = 100;

  enum class Layer { background_layer, board_layer };

 public:
  view(icanvas& c) : canvas_(c) {
    canvas_.draw(canvas_.load_image("images/background.png"), 0, 0, 0);

    for (int i = 1; i <= 5; ++i) {
      grids[i - 1] = canvas_.load_image("images/" + std::to_string(i) + ".png");
    };

    canvas_.render();
  }

  void show_grid(int x, int y, int c) {
    canvas_.draw(grids[c - 1], grids_offset_x + (x * grid_size), grids_offset_y + (y * grid_size), 1);
    canvas_.render();
  }

  std::array<std::shared_ptr<SDL_Texture>, 5> grids;
  icanvas& canvas_;
};

class board {};

struct button_clicked {
  static constexpr auto id = SDL_MOUSEBUTTONUP;
};
struct key_pressed {
  static constexpr auto id = SDL_KEYDOWN;
};
struct window_closed {
  static constexpr auto id = SDL_QUIT;
};

auto guard = [] { return true; };
template <int Key>
auto is_key = [](auto e) { return true; };

auto init_board = [](view& v) {
  std::random_device rd;
  std::mt19937 eng(rd());
  std::uniform_int_distribution<> rand(1, 5);
  for (auto i = 0; i < 9; ++i) {
    for (auto j = 0; j < 9; ++j) {
      v.show_grid(i, j, rand(eng));
    }
  }
};

struct controller {
  auto configure() const noexcept {
    using namespace msm;
    // clang-format off
    return make_transition_table(
     // +-----------------------------------------------------------------+
        "idle"_s(initial) / init_board
      , "idle"_s == "s1"_s + event<button_clicked> / [] { std::cout << "clicked" << std::endl; }
     // +-----------------------------------------------------------------+
      , "wait_for_client"_s(initial) == terminate + event<window_closed>
      , "wait_for_client"_s == terminate + event<key_pressed> [is_key<SDLK_ESCAPE>]
     // +-----------------------------------------------------------------+
    );
    // clang-format on
  }
};

#if __has_include(<SDL.h>)&&__has_include(<SDL_image.h>)
class sdl_user_input : public iclient {
 public:
  explicit sdl_user_input(msm::sm<controller>& c) : controller_(c) {}

  void run() override {
#if defined(EMSCRIPTEN)
    emscripten_set_main_loop_arg(run_impl, (void*)&controller_, 0, 0);
#else
    run_impl((void*)&controller_);
#endif
  }

  static void run_impl(void* c) {
    auto& controller_ = (msm::sm<controller>&)*c;
#if !defined(EMSCRIPTEN)
    while (!controller_.is(msm::terminate)) {
#endif
      SDL_Event event = {};
      auto dispatch_event = msm::make_dispatch_table<SDL_Event, SDL_FIRSTEVENT, 10 /*SDL_LASTEVENT*/>(controller_);
      while (SDL_PollEvent(&event)) {
        dispatch_event(event, event.type);
      }
#if !defined(EMSCRIPTEN)
    }
#endif
  }

 private:
  msm::sm<controller>& controller_;
};
#endif

class game {
 public:
  game(msm::sm<controller>& c, iclient& cl) : controller_(c), client_(cl) {}

  void play() { client_.run(); }

  iclient& client_;
  msm::sm<controller>& controller_;
};

auto configuration = [] {
  return di::make_injector(di::bind<icanvas>.to<sdl_canvas>(), di::bind<iclient>.to<sdl_user_input>());
};

}  // match3

int main() {
  auto injector = di::make_injector(match3::configuration());
  injector.create<match3::game>().play();
}
