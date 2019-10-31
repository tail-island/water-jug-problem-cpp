#pragma once

#include <queue>
#include <unordered_set>

#include "game.hpp"

namespace water_jug_problem {
  class breadth_first_search final {
    struct node final {
      water_jug_problem::state state;
      std::vector<action>      answer;
    };

    const game& _game;

    auto next_answer(const std::vector<action>& answer, const action& action) const noexcept {
      auto result = std::vector<water_jug_problem::action>(); result.reserve(answer.size() + 1);

      result.assign(std::begin(answer), std::end(answer));
      result.emplace_back(action);

      return result;
    }

  public:
    explicit breadth_first_search(const game& game) noexcept: _game(game) {
      ;
    }

    auto operator()() const noexcept {
      auto queue   = std::queue<node>();
      auto visited = std::unordered_set<state>();

      queue.emplace(node{_game.initial_state(), std::vector<action>(0)});
      visited.emplace(_game.initial_state());

      while (!std::empty(queue)) {
        const auto& node = queue.front();

        for (const auto& action: _game.legal_actions(node.state)) {
          const auto& next_state = _game.next_state(node.state, action);

          if (!visited.emplace(next_state).second) {
            continue;
          }

          const auto& next_answer = breadth_first_search::next_answer(node.answer, action);

          if (_game.is_goal(next_state)) {
            return next_answer;
          }

          queue.emplace(breadth_first_search::node{next_state, next_answer});
        }

        queue.pop();
      }

      return std::vector<action>();
    }
  };
}
