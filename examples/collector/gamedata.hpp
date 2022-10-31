#ifndef GAMEDATA_HPP_
#define GAMEDATA_HPP_

#include <bitset>
#include <vector>

using std::vector;

enum class Input { Right, Left, Pause, Enter };
enum class State { Playing, GameOver, Paused, New };

struct GameData {
  State m_state{State::Playing};
  std::bitset<4> m_input;  // [left, right, pause, enter]
  int m_points{0};
  int m_sides{100};
  int m_qtdObjects{8};
  float m_scale{0.08f};
  vector<vector<float>> m_objects;
};

#endif