#include "Objects.hpp"
#include <iostream>
#include <time.h>
#include <vector>

void Objects::create(GLuint program) {
  m_program = program;
  m_timer.restart();
}

void Objects::paint(GameData &gameData) {
    int qtdObjects = gameData.m_objects.size();
    int qtdMaxObjects = gameData.m_qtdObjects;

    m_randomEngine.seed(std::chrono::steady_clock::now().time_since_epoch().count());

    //Durante o jogo sempre haverá uma quantidade fixa de objeto renderizado de acordo com a variação qtdMaxObjects
    //Caso a quantidade seja menor que a esperada, seja por ter iniciado o jogo ou por termos coletado algum objeto, criamos novos no início da tela referente ao eixo y
    // e escolhemos a posição no eixo x de forma randômica
    if(qtdObjects < qtdMaxObjects){
      long long unsigned int news = qtdMaxObjects - qtdObjects;
      for(long long unsigned int i = 0; i < news; i++){
        std::uniform_real_distribution realDist(-0.9, 0.9);
        float xPos = realDist(m_randomEngine);
        gameData.m_objects.push_back({xPos, 1.0f});
      }
    }

    //Renderização de cada objeto do array  
    for (long long unsigned int i = 0; i < gameData.m_objects.size(); i++){
      std::uniform_int_distribution intDist(0, 2);
      int RanIndex = intDist(m_randomEngine);
      m_currentColor = glm::make_vec3(colors[RanIndex]);

      setupModel(gameData.m_sides);

      abcg::glUseProgram(m_program);
      
      auto const translationLocation{abcg::glGetUniformLocation(m_program, "translation")};
      abcg::glUniform2fv(translationLocation, 1, &gameData.m_objects.at(i).at(0));
      
      auto const scaleLocation{abcg::glGetUniformLocation(m_program, "scale")};
      abcg::glUniform1f(scaleLocation, gameData.m_scale);

      abcg::glBindVertexArray(m_VAO);
      abcg::glDrawArrays(GL_TRIANGLE_FAN, 0, 102);
      abcg::glBindVertexArray(0);
      abcg::glUseProgram(0);
    }
}

void Objects::destroy() {
  abcg::glDeleteProgram(m_program);
  abcg::glDeleteBuffers(1, &m_VBOPositions);
  abcg::glDeleteBuffers(1, &m_VBOColors);
  abcg::glDeleteVertexArrays(1, &m_VAO);
}

//A cada chamada da função update() verificamos se há objetos e se está no tempo correto para atualiza-los de forma que cada um tem sua posição y atualizada, para
// que durante o jogo estejam caindo até o início da tela. A distancia com que o objeto cai a cada atualização depende da quantidade de pontos, assim quanto mais 
// pontos, mais rapido o objeto cairá. Se a pontuação for negativa, o jogo encerra.
void Objects::update(GameData &gameData, int delay) {
  int qtdObjects = gameData.m_objects.size();

    if (m_timer.elapsed() >= delay / 10000.0 && qtdObjects > 0){
      m_timer.restart();
      if (gameData.m_points < 0){
        gameData.m_state = State::GameOver;
        gameData.m_points = 0;
        gameData.m_objects.clear();
      } else if(gameData.m_points < 50){
          updateYAxis(0.01, gameData);
      } else if (gameData.m_points  < 120){
          updateYAxis(0.03, gameData);
      } else if (gameData.m_points >= 120){
          updateYAxis(0.06, gameData);
      }
    }
}

void Objects::setupModel(int sides) {

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

//Função é chamada para que a posição de cada objeto no eixo y seja atualizada, ou seja, faz com os objetos "caiam" na do início da tela até o final
//Caso o objeto já esteja no limite da tela, ele é removido do array e 20 pontos é descontado do jogador
void Objects::updateYAxis(double minY, GameData &gameData) {
  for (long long unsigned int i = 0; i < gameData.m_objects.size(); i++){
    if(gameData.m_objects.at(i).at(1) < -1.0f){
        gameData.m_objects.erase(gameData.m_objects.begin()+i, gameData.m_objects.begin()+i+1);
        gameData.m_points = gameData.m_points - 10;
    }else{
        std::uniform_real_distribution realDist(minY, 0.08);
        gameData.m_objects.at(i).at(1) = gameData.m_objects.at(i).at(1) - realDist(m_randomEngine);
    }
  }
}

