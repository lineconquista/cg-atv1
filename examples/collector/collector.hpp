#ifndef COLLECTOR_HPP_
#define COLLECTOR_HPP_

#include "abcgOpenGL.hpp"
#include "gamedata.hpp"

class Collector {
public:
  void create(GLuint program);
  void paint(const GameData &gameData);
  void destroy();
  void update(GameData &gameData);

private:
  GLuint m_VAO{};
  GLuint m_VBOPositions{};
  GLuint m_VBOColors{};
  GLuint m_program{};

  glm::vec2 m_translation{0, -0.92f};
  glm::vec3 m_currentColor{1.0f, 0.4f, 0.69f};

  abcg::Timer m_timer;

  int m_delay{300};
  float m_distance{0.0f};

  void setupModel(int sides);
  void check(GameData &gameData);
};

#endif