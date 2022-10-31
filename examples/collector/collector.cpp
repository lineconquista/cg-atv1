#include "collector.hpp"
#include <iostream>

void Collector::create(GLuint program) {
  m_program = program;
  m_timer.restart();
}

void Collector::paint(const GameData &gameData) {
  setupModel(gameData.m_sides);

  abcg::glUseProgram(m_program);
  
  auto const translationLocation{abcg::glGetUniformLocation(m_program, "translation")};
  abcg::glUniform2fv(translationLocation, 1, &m_translation.x);

  auto const scaleLocation{abcg::glGetUniformLocation(m_program, "scale")};
  abcg::glUniform1f(scaleLocation, gameData.m_scale);

  abcg::glBindVertexArray(m_VAO);
  abcg::glDrawArrays(GL_TRIANGLE_FAN, 0, gameData.m_sides + 2);
  abcg::glBindVertexArray(0);
  abcg::glUseProgram(0);
}

void Collector::destroy() {
  abcg::glDeleteProgram(m_program);
  abcg::glDeleteBuffers(1, &m_VBOPositions);
  abcg::glDeleteBuffers(1, &m_VBOColors);
  abcg::glDeleteVertexArrays(1, &m_VAO);
}

void Collector::update(GameData &gameData) {
  //Movimento para lado esquerdo
  if (gameData.m_input[gsl::narrow<size_t>(Input::Left)]){
      m_translation.x = m_translation.x <=-0.92f ? -0.92f : m_translation.x - 0.003f - (m_distance * 0.003f);
      m_distance += 0.80f;
  }
  //Movimento para lado direito
  else if (gameData.m_input[gsl::narrow<size_t>(Input::Right)]){
      m_translation.x = m_translation.x >= 0.92f ? 0.92f : m_translation.x + 0.003f + (m_distance * 0.003f);
      m_distance += 0.80f;
  }else{
    m_distance = 0.00f;
  }
  check(gameData);
}

void Collector::setupModel(int sides) {
  abcg::glDeleteBuffers(1, &m_VBOPositions);
  abcg::glDeleteBuffers(1, &m_VBOColors);
  abcg::glDeleteVertexArrays(1, &m_VAO);

  std::vector<glm::vec2> positions;
  std::vector<glm::vec3> colors;

  positions.emplace_back(0, 0);
  colors.push_back(m_currentColor);

  auto const step{M_PI * 2 / sides};
  for (auto const angle : iter::range(0.0, M_PI * 2, step)) {
    positions.emplace_back(std::cos(angle), std::sin(angle));
    colors.push_back(m_currentColor);
  }

  positions.push_back(positions.at(1));
  colors.push_back(m_currentColor);

  abcg::glGenBuffers(1, &m_VBOPositions);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_VBOPositions);
  abcg::glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec2), positions.data(), GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  abcg::glGenBuffers(1, &m_VBOColors);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_VBOColors);
  abcg::glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(glm::vec3), colors.data(), GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  auto const positionAttribute{ abcg::glGetAttribLocation(m_program, "inPosition")};
  auto const colorAttribute{abcg::glGetAttribLocation(m_program, "inColor")};

  abcg::glGenVertexArrays(1, &m_VAO);

  abcg::glBindVertexArray(m_VAO);

  abcg::glEnableVertexAttribArray(positionAttribute);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_VBOPositions);
  abcg::glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  abcg::glEnableVertexAttribArray(colorAttribute);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_VBOColors);
  abcg::glVertexAttribPointer(colorAttribute, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  abcg::glBindVertexArray(0);
}

//Valida se colidiu com algum dos objetos, caso tenha ocorrido, acrescenta 10 pontos
void Collector::check(GameData &gameData){
  for (long long unsigned int i = 0; i < gameData.m_objects.size(); i++){   
    glm::vec2 m_translationObject{gameData.m_objects.at(i).at(0), gameData.m_objects.at(i).at(1)};
    auto const distance{glm::distance(m_translation, m_translationObject)};
    if (distance < gameData.m_scale * 0.9f + gameData.m_scale * 0.85f) {
      gameData.m_objects.erase(gameData.m_objects.begin()+i, gameData.m_objects.begin()+i+1);
      gameData.m_points = gameData.m_points + 15;
    }
  }
}