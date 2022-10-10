#ifndef WINDOW_HPP_
#define WINDOW_HPP_

#include "abcgOpenGL.hpp"
#include <vector>
using std::string;
using std::vector;

class Window : public abcg::OpenGLWindow {
protected:
  void onCreate() override;
  void onPaintUI() override;
  
private:
  static int const column{4};
  static int const line{5};
  static int const maxDigits{30};
  static constexpr char buttons[maxDigits] = {'c', 'x', '%', '/', '7', '8', '9', '*', '4', '5', '6', '-', '1', '2', '3', '+', '0', ' ', ' ', '='};
  int pos = 0;
  
  enum class GameState { Finished, Running };
  GameState m_calcState;

  std::array<char, maxDigits> digits {};
  std::array<char, column * line> m_board{};
  ImFont *m_font{};

  float getResult();
  void startCalc();
  string getExpression();
  bool checkLastNumber();
  vector<float> getNumbers();
  vector<char> getOperators();
  float calc(vector<char> operators, vector<float> numbers);

};

#endif