#include "window.hpp"

int main(int argc, char **argv) {
  try {
    abcg::Application app(argc, argv);

    Window window;
    window.setOpenGLSettings({.samples = 2, .doubleBuffering = true});
    window.setWindowSettings({.width = 700, .height = 700, .showFPS = false, .showFullscreenButton = false, .title = "Regular Polygons"});
    app.run(window);
    
  } catch (std::exception const &exception) {
    fmt::print(stderr, "{}\n", exception.what());
    return -1;
  }
  return 0;
}
