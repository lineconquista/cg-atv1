#ifndef OBJECTS_HPP_
#define OBJECTS_HPP_

#include <random>
#include "abcgOpenGL.hpp"
#include "gamedata.hpp"
#include <glm/gtc/type_ptr.hpp>

class Objects {
public:
  void create(GLuint program);
  void paint(GameData &gameData);
  void destroy();
  void update(GameData &gameData, int delay);

private:
  GLuint m_VAO{};
  GLuint m_VBOPositions{};
  GLuint m_VBOColors{};
  GLuint m_program{};

  //Distância que é percorrida a cada vez que o movimento é acionado para esquerda ou direita
  float m_distance{0.0f};

  //Array de cores que serão utilizadas nos objetos
  float colors[3][3] = {{0.72f, 0.80f, 0.91f}, {0.91f, 0.72f, 0.88f}, {0.82f, 0.72f, 0.91f}};

  glm::vec2 m_translation{0.0f, 1.0f};
  glm::vec3 m_currentColor = glm::make_vec3(colors[0]);

  std::default_random_engine m_randomEngine;
  abcg::Timer m_timer;
  
  void setupModel(int sides);
  void updateYAxis(double minY, GameData &gameData);
};

#endif