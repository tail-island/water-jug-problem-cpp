#pragma once

#include <tuple>
#include <vector>

namespace water_jug_problem {
  using action = std::tuple<int, int>;  // 面倒なので型エイリアスで……。
  using state  = std::vector<int>;      // 面倒なので型エイリアスで……。

  class game final {
    std::vector<int> _pitcher_capacities;

  public:
    game(std::initializer_list<int> pitcher_capacities) noexcept: _pitcher_capacities(pitcher_capacities) {
      ;
    }

    const auto& pitcher_capacities() const noexcept {
      return _pitcher_capacities;
    }

    auto initial_state() const noexcept {
      auto result = std::vector<int>(std::size(pitcher_capacities()));

      result[0] = pitcher_capacities()[0];

      return result;
    }

    auto legal_actions(const state& state) const noexcept {
      auto result = std::vector<action>();

      for (auto i = 0; i < static_cast<int>(std::size(pitcher_capacities())); ++i) {
        for (auto j = 0; j < static_cast<int>(std::size(pitcher_capacities())); ++j) {
          if (i != j && state[i] && state[j] != pitcher_capacities()[j]) {
            result.emplace_back(i, j);
          }
        }
      }

      return result;
    }

    auto next_state(const state& state, const action& action) const noexcept {
      auto result = state;

      const auto& [i, j] = action;

      result[i] = std::max(state[i] - (pitcher_capacities()[j] - state[j]), 0);
      result[j] = std::min(state[j] + state[i], pitcher_capacities()[j]);

      return result;
    }

    auto is_goal(const state& state) const noexcept {
      for (const auto& pitcher: state) {
        if (pitcher == pitcher_capacities()[0] / 2) {
          return true;
        }
      }

      return false;
    }
  };
}

namespace std{
  template<>
  struct hash<water_jug_problem::state> {
    auto operator()(const water_jug_problem::state& state) const noexcept {  // FNV-1aで作ってみたけど自信ない。誰かこれで正しいのか教えて！
      auto result = 14695981039346656037UL;

      for (const auto& pitcher: state) {
        result = (result ^ (pitcher >> 24 & 0xff)) * 1099511628211UL;
        result = (result ^ (pitcher >> 16 & 0xff)) * 1099511628211UL;
        result = (result ^ (pitcher >>  8 & 0xff)) * 1099511628211UL;
        result = (result ^ (pitcher       & 0xff)) * 1099511628211UL;
      }

      return result;
    }
  };
}
