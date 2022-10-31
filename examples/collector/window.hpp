#ifndef WINDOW_HPP_
#define WINDOW_HPP_

#include <random>
#include "abcgOpenGL.hpp"
#include "collector.hpp"
#include "starlayers.hpp"
#include "objects.hpp"

class Window : public abcg::OpenGLWindow {
protected:
  void onEvent(SDL_Event const &event) override;
  void onCreate() override;
  void onUpdate() override;
  void onPaint() override;
  void onPaintUI() override;
  void onResize(glm::ivec2 const &size) override;
  void onDestroy() override;

private:
  glm::ivec2 m_viewportSize{};

  GLuint m_starsProgram{};
  GLuint m_objectsProgram{};
  GLuint m_collectorProgram{};

  GameData m_gameData;

  int m_delay{300};
  
  ImFont *m_font{};

  Collector m_collector;
  StarLayers m_starLayers;
  Objects m_objects;

  void restart();
};

#endif