#include "window.hpp"
#include <string> 

using std::string;

void Window::onEvent(SDL_Event const &event) {
  // Eventos do teclado
  if (event.type == SDL_KEYDOWN) {
    if (event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_a)
      m_gameData.m_input.set(gsl::narrow<size_t>(Input::Left));
    if (event.key.keysym.sym == SDLK_RIGHT || event.key.keysym.sym == SDLK_d)
      m_gameData.m_input.set(gsl::narrow<size_t>(Input::Right));
    if (event.key.keysym.sym == SDLK_p)
      m_gameData.m_input.set(gsl::narrow<size_t>(Input::Pause));
    if (event.key.keysym.sym == SDLK_KP_ENTER || event.key.keysym.sym == SDLK_RETURN){
      if (m_gameData.m_state == State::New || m_gameData.m_state == State::GameOver){
        m_gameData.m_state = State::Playing;
      }
    }
    if (event.key.keysym.sym == SDLK_p){
      if (m_gameData.m_state == State::Paused){
        m_gameData.m_state = State::Playing;
      }else if (m_gameData.m_state == State::Playing){
        m_gameData.m_state = State::Paused;
      }
    }
  }
  if (event.type == SDL_KEYUP) {
    if (event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_a)
      m_gameData.m_input.reset(gsl::narrow<size_t>(Input::Left));
    if (event.key.keysym.sym == SDLK_RIGHT || event.key.keysym.sym == SDLK_d)
      m_gameData.m_input.reset(gsl::narrow<size_t>(Input::Right));
  }
}

void Window::onCreate() {
  auto const assetsPath{abcg::Application::getAssetsPath()};

  auto const filename{abcg::Application::getAssetsPath() + "Inconsolata-Medium.ttf"};
  m_font = ImGui::GetIO().Fonts->AddFontFromFileTTF(filename.c_str(), 30.0f);
  if (m_font == nullptr) {
    throw abcg::RuntimeError{"Cannot load font file"};
  }

  m_starsProgram = abcg::createOpenGLProgram({{.source = assetsPath + "stars.vert", .stage = abcg::ShaderStage::Vertex}, {.source = assetsPath + "stars.frag", .stage = abcg::ShaderStage::Fragment}});
  m_objectsProgram = abcg::createOpenGLProgram({{.source = assetsPath + "main.vert", .stage = abcg::ShaderStage::Vertex}, {.source = assetsPath + "main.frag", .stage = abcg::ShaderStage::Fragment}});
  m_collectorProgram = abcg::createOpenGLProgram({{.source = assetsPath + "main.vert", .stage = abcg::ShaderStage::Vertex}, {.source = assetsPath + "main.frag", .stage = abcg::ShaderStage::Fragment}});
  abcg::glClearColor(154.0f/255.0f, 122.0f/255.0f, 204.0f/255.0f, 0);

#if !defined(__EMSCRIPTEN__)
  abcg::glEnable(GL_PROGRAM_POINT_SIZE);
#endif

  restart();
}

void Window::restart() {
  m_gameData.m_state = State::New;
  m_starLayers.create(m_starsProgram, 25);
  m_collector.create(m_collectorProgram);
  m_objects.create(m_objectsProgram);
}

void Window::onUpdate() {
  //Background continua sendo renderizado e atualizado em todos estados do jogo
  auto const deltaTime{gsl::narrow_cast<float>(getDeltaTime())};
  m_starLayers.update(deltaTime);

  //Demais elementos só são atualizados durante o estado Playing
  if (m_gameData.m_state == State::Playing){
    m_collector.update(m_gameData);
    m_objects.update(m_gameData, m_delay);
  }

}

void Window::onPaint() {
  abcg::glClear(GL_COLOR_BUFFER_BIT);
  abcg::glViewport(0, 0, m_viewportSize.x, m_viewportSize.y);
  
  //Background continua sendo renderizado e atualizado em todos estados do jogo
  m_starLayers.paint();

  //Demais elementos só são atualizados durante o estado Playing
  if (m_gameData.m_state == State::Playing){
      m_collector.paint(m_gameData);
      m_objects.paint(m_gameData);
  }
}

void Window::onPaintUI() {
  abcg::OpenGLWindow::onPaintUI();

  {
    auto const size{ImVec2(300, 85)};
    ImGui::SetNextWindowSize(size);
    ImGuiWindowFlags const flags{ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoInputs};
    ImGui::PushFont(m_font);
        
 
    if(m_gameData.m_state != State::Playing){
      string status, message;
      //Mensagens exibidas na tela de acordo com o estado do jogo

      if(m_gameData.m_state == State::New){
        status = "Welcome >.<";
        message = "Press Enter to start!";
      }
      if(m_gameData.m_state == State::GameOver){
        status = "GameOver =(";
        message = "Press Enter to start!";
      }
      if(m_gameData.m_state == State::Paused){
        status = "Paused '-'";
        message = "Press P to return!";
      }
      
      auto const position{ImVec2((m_viewportSize.x - size.x) / 2.0f, (m_viewportSize.y - size.y) / 2.0f)};
      ImGui::SetNextWindowPos(position);
      float font_size = ImGui::GetFontSize() * status.size() / 2;
      ImGui::Begin(" ", nullptr, flags);
      ImGui::SameLine(ImGui::GetWindowSize().x / 2 - font_size + (font_size / 2));
      ImGui::Text("%s", status.c_str());
      ImGui::Text("%s", message.c_str());
      ImGui::PopFont();
      ImGui::End();
    }else{
        string points = "points: " + std::to_string(m_gameData.m_points); 
        auto const position{ImVec2((m_viewportSize.x - size.x)/2.0f, (m_viewportSize.y - size.y)/30.0f)};
        ImGui::SetNextWindowPos(position);
        float font_size = ImGui::GetFontSize() * points.size() / 2;
        ImGui::Begin(" ", nullptr, flags);
        ImGui::SameLine(ImGui::GetWindowSize().x / 2 - font_size + (font_size / 2));
        ImGui::Text("%s", points.c_str());
        ImGui::PopFont();
        ImGui::End();
    }
    
    ImGui::End();
  }
}

void Window::onResize(glm::ivec2 const &size) {
  m_viewportSize = size;
  abcg::glClear(GL_COLOR_BUFFER_BIT);
}

void Window::onDestroy() {
  abcg::glDeleteProgram(m_starsProgram);
  abcg::glDeleteProgram(m_objectsProgram);
  m_collector.destroy();
  m_starLayers.destroy();
}