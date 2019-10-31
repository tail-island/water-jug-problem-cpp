#include <iostream>

#include "beam_search.hpp"
#include "best_first_search.hpp"
#include "breadth_first_search.hpp"
#include "game.hpp"

auto print_answer(const std::string& caption, const water_jug_problem::game& game, const std::vector<water_jug_problem::action>& answer) noexcept {
  auto state = game.initial_state();

  std::cout << caption << std::endl;

  for (const auto& action: answer) {
    state = game.next_state(state, action);

    std::cout << "(" << std::get<0>(action) << ", " << std::get<1>(action) << ")" << ", " << "[" << state[0] << ", " << state[1] << ", " << state[2] << "]" << std::endl;
  }

  std::cout << std::endl;
}

int main(int argc, char** argv) {
  auto game = water_jug_problem::game{8, 5, 3};

  print_answer("breadth first search", game, water_jug_problem::breadth_first_search(game)());
  print_answer("best first search",    game, water_jug_problem::best_first_search(game)());
  print_answer("beam search",          game, water_jug_problem::beam_search(game, 2)());

  return 0;
}
