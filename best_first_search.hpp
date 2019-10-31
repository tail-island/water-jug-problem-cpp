#pragma once

#include <queue>
#include <unordered_set>

#include "game.hpp"

namespace water_jug_problem {
  class best_first_search final {
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

    auto next_answer(const std::vector<action>& answer, const action& action) const noexcept {
      auto result = std::vector<water_jug_problem::action>(); result.reserve(answer.size() + 1);

      result.assign(std::begin(answer), std::end(answer));
      result.emplace_back(action);

      return result;
    }

    auto score(const state& state, const std::vector<action>& answer) const noexcept {
      auto result = 0;

      // ここを良い感じに変えてみてください。

      result -= std::size(answer);

      for (const auto& pitcher: state) {
        if (pitcher % (_game.pitcher_capacities()[0] / 2) == 0) {
          result += 10;
        }

        result -= std::abs(_game.pitcher_capacities()[0] / 2 - pitcher);
      }

      return result;
    }

  public:
    explicit best_first_search(const game& game) noexcept: _game(game) {
      ;
    }

    auto operator()() const noexcept {
      auto queue   = std::priority_queue<node, std::vector<node>, node_comparer>();
      auto visited = std::unordered_set<state>();

      queue.emplace(node{_game.initial_state(), std::vector<action>(0), 0});
      visited.emplace(_game.initial_state());

      while (!std::empty(queue)) {
        const auto node = queue.top(); queue.pop();  // キューが更新されるので順序が変わるかもしれないし、あと、std::vectorでキューを作っているのでデータ移動するかもしれないので、コピーしてpop()しないとダメなんじゃないかなぁと……。

        for (const auto& action: _game.legal_actions(node.state)) {
          const auto& next_state = _game.next_state(node.state, action);

          if (!visited.emplace(next_state).second) {
            continue;
          }

          const auto& next_answer = best_first_search::next_answer(node.answer, action);

          if (_game.is_goal(next_state)) {
            return next_answer;
          }

          queue.emplace(best_first_search::node{next_state, next_answer, score(next_state, next_answer)});
        }
      }

      return std::vector<action>();
    }
  };
}
