#include "window.hpp"
#include <string> 
#include <vector>
#include <sstream> 
#include <iostream>

using std::string;
using std::vector;

void Window::startCalc() {
  digits.fill('\0');
  m_calcState = GameState::Running;
  pos = 0;
}

vector<float> Window::getNumbers(){
  vector<float> n;
  string num = "";
  for(int i = 0; i < 20; i++){
      char keyValue = digits.at(i);
      if (isdigit(keyValue)){
        num += keyValue;
        //proximo é operador
        if(!isdigit(digits.at(i+1))){
          n.push_back(stof(num));
          num = "";
        }
      }
  }
  return n;
}

vector<char> Window::getOperators(){
  vector<char> operators;
  for(int i = 0; i < 20; i++){
      char keyValue = digits.at(i);
      if (!isdigit(keyValue) && keyValue != '\0'){
        operators.push_back(keyValue);
      }
  }
  return operators;
}

float Window::calc(vector<char> operators, vector<float> numbers){
  vector<char> newOperators;
  vector<float> newNumbers;

  for (long long unsigned int i = 0; i < operators.size(); i++){
    char operatorKey = operators.at(i);
    float res;
    if(operatorKey=='/' || operatorKey=='*' || operatorKey=='%'){
      float firstNum = numbers.at(i) == '\0' ?  numbers.at(i+1) :  numbers.at(i);
      float lastNum = numbers.at(i) == '\0' ?  numbers.at(i+2) :  numbers.at(i+1) ;

      if(operatorKey=='/'){
        res = firstNum / lastNum;
      }

      if(operatorKey=='*'){
        res = firstNum * lastNum;
      }

      if(operatorKey=='%'){
        res = firstNum * lastNum/100;
      }
      
      if(i+1 <  operators.size() && (operators.at(i+1) == '\0' || operators.at(i+1) == '+' || operators.at(i+1) == '-' )){
        newNumbers.push_back(res);
        numbers.at(i+1) = '\0';
      }else if ((i + 1) >= operators.size()){
        newNumbers.push_back(res);
      }else{
        numbers.at(i+1) = res;
      }

      
    }else{
      newOperators.push_back(operatorKey);
      if(numbers.at(i) != '\0'){
          newNumbers.push_back(numbers.at(i));
      }
  
      if(i==operators.size()-1){
         newNumbers.push_back(numbers.at(i+1));
      }
    }
  }

  float result = newNumbers.at(0);
  for (long long unsigned int i = 0; i < newOperators.size(); i++){
    char operatorKey = newOperators.at(i);
    float lastNum = newNumbers.at(i+1);
    if(operatorKey=='+'){
        result += lastNum;
    }

    if(operatorKey=='-'){
        result -= lastNum;
    }
  }
  return result;
}

float Window::getResult() {
  vector<char> operators = getOperators();
  vector<float> numbers = getNumbers();
  return calc(operators, numbers);
}

string Window::getExpression(){
  string text = "";

  switch (m_calcState) {
      case GameState::Running:
        for(int i = 0; i < maxDigits; i++){
          if( digits.at(i) == '\0'){
            break;
          }
          text += digits.at(i);
        }
        break;
      case GameState::Finished:
        std::stringstream stream;  
        stream.precision(3);
        stream << std::fixed;
        stream<<getResult();  
        text = stream.str();  
        break;
  }
  return text;
}

bool Window::checkLastNumber(){
  return pos == 0 ? false : isdigit(digits[pos-1]) ;
}

void Window::onCreate() {
  auto const filename{abcg::Application::getAssetsPath() + "Inconsolata-Medium.ttf"};
  m_font = ImGui::GetIO().Fonts->AddFontFromFileTTF(filename.c_str(), 30.0f);
  if (m_font == nullptr) {
    throw abcg::RuntimeError{"Cannot load font file"};
  }
  startCalc();
}

void Window::onPaintUI() {

  auto const appWindowWidth{gsl::narrow<float>(getWindowSettings().width)};
  auto const appWindowHeight{gsl::narrow<float>(getWindowSettings().height)};
  ImGui::PushStyleColor(ImGuiCol_WindowBg, (ImVec4)ImColor(0, 0, 0));

  {
    ImGui::SetNextWindowSize(ImVec2(appWindowWidth, appWindowHeight));
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    auto const flags{ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse};
    ImGui::Begin("", nullptr, flags);

    //Static text from calculator input
    {
      ImGui::PushFont(m_font);
      string text = getExpression();
      ImGui::SetCursorPosX((appWindowWidth - ImGui::CalcTextSize(text.c_str()).x) / 2);
      ImGui::Text("%s", text.c_str());
    }
    
    //Calculator keys
    {
      auto const gridHeight{appWindowHeight - 100};
      auto const buttonHeight{gridHeight / line};
      auto const tableFlags{  ImGuiTableFlags_NoPadOuterX | ImGuiTableFlags_NoPadInnerX };

      ImGui::PushFont(m_font);
      
      if (ImGui::BeginTable("Numbers", column, tableFlags)) {

        for (auto i : iter::range(line)) {
          ImGui::TableNextRow();

          for (auto j : iter::range(column)) {

            ImGui::TableSetColumnIndex(j);
            auto const offset{i * column + j};
            auto ch{m_board.at(offset)};
            auto buttonText{fmt::format("{}", ch)};
            char valueKey = buttons[i*4+j];

            m_board.at(offset) = valueKey;
      
            ImGui::PushID(i);

            if(valueKey=='='){     
              ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor(192, 136, 170));
              ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor(218, 154, 192));
              ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0);   

              if(ImGui::Button(buttonText.c_str(), ImVec2(-1, buttonHeight))){
                if(checkLastNumber() && getNumbers().size() > 1){
                  m_calcState = GameState::Finished;
                }
              }

              ImGui::PopStyleColor(2);
            
            }else if(valueKey=='x'){
              ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0);  

              if (ImGui::Button(buttonText.c_str(), ImVec2(-1, buttonHeight))) {
                switch (m_calcState) {
                    case GameState::Running:
                      if(pos!=0){
                        pos = pos - 1;
                        digits.at(pos)= '\0';
                      }
                      break;
                    case GameState::Finished:
                      startCalc();
                      break;
                }
              }
            }else if(valueKey=='c'){
              ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0);  

              if (ImGui::Button(buttonText.c_str(), ImVec2(-1, buttonHeight))) {
                  startCalc();
              }
            }else if(!isdigit(valueKey)){
              ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0);

              if (ImGui::Button(buttonText.c_str(), ImVec2(-1, buttonHeight)) && valueKey != ' ') {

                if(m_calcState == GameState::Finished){
                      startCalc();
                }

                if(checkLastNumber()){
                    digits.at(pos)= valueKey;
                    pos += 1;
                }else{
                    pos = pos == 0 ? pos : pos - 1;
                    digits.at(pos);
                }

              }
            }else{                      
              ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0);    
              if (ImGui::Button(buttonText.c_str(), ImVec2(-1, buttonHeight))) {

                if(m_calcState == GameState::Finished){
                      startCalc();
                }

                if(pos==20){
                  digits.at(pos)= valueKey;
                }else if (pos != 20 && !(valueKey == '0' && digits.at(pos-1) == '/')){
                    digits.at(pos)= valueKey;
                    pos += 1;
                }
              }           
            }
            ImGui::PopStyleVar(1);
            ImGui::PopID();        
          }
        }
        ImGui::EndTable();
      }
      ImGui::PopFont();
    }
    ImGui::End();
  }
}

