#pragma once

#include <queue>
#include <unordered_set>

#include "game.hpp"

namespace water_jug_problem {
  class beam_search final {
    struct node final {
      water_jug_problem::state state;
      std::vector<action>      answer;
      int                      score;
    };

    struct node_comparer final {
      auto operator()(const node& node_1, const node& node_2) const noexcept {
        return node_1.score < node_2.score;
      }
    };

    const game& _game;
    const int   _beam_width;

    auto next_answer(const std::vector<action>& answer, const action& action) const noexcept {
      auto result = std::vector<water_jug_problem::action>(); result.reserve(answer.size() + 1);

      result.assign(std::begin(answer), std::end(answer));
      result.emplace_back(action);

      return result;
    }

    auto score(const state& state) const noexcept {
      auto result = 0;

      for (const auto& pitcher: state) {
        if (pitcher % (_game.pitcher_capacities()[0] / 2) == 0) {
          result += 10;
        }

        result -= std::abs(_game.pitcher_capacities()[0] / 2 - pitcher);
      }

      return result;
    }

  public:
    explicit beam_search(const game& game, int beam_width) noexcept: _game(game), _beam_width(beam_width) {
      ;
    }

    auto operator()() const noexcept {
      auto queue   = std::priority_queue<node, std::vector<node>, node_comparer>();
      auto visited = std::unordered_set<state>();

      queue.emplace(node{_game.initial_state(), std::vector<action>(0), 0});
      visited.emplace(_game.initial_state());

      while (!std::empty(queue)) {
        auto next_queue = std::priority_queue<node, std::vector<node>, node_comparer>();

        for (auto i = 0; i < _beam_width && !std::empty(queue); ++i) {
          const auto& node = queue.top();

          for (const auto& action: _game.legal_actions(node.state)) {
            const auto& next_state = _game.next_state(node.state, action);

            if (!visited.emplace(next_state).second) {
              continue;
            }

            const auto& next_answer = beam_search::next_answer(node.answer, action);

            if (_game.is_goal(next_state)) {
              return next_answer;
            }

            next_queue.emplace(beam_search::node{next_state, next_answer, score(next_state)});
          }

          queue.pop();
        }

        queue = next_queue;
      }

      return std::vector<action>();
    }
  };
}
