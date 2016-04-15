/**
 * @file state.hpp
 * @brief header of AbstractState class
 */

#pragma once

#include <list>

namespace mcts {
template <class ScoreType>
struct AbstractState {
  using score_t = typename ScoreType;

  AbstractState() = default;
  AbstractState(const AbstractState&) = default;
  AbstractState(AbstractState&&) noexcept = default;
  virtual ~AbstractState() = default;

  auto operator=(const AbstractState&) -> AbstractState& = default;
  auto operator=(AbstractState&&) -> AbstractState& = default;

  // Please override the following functions
  template <class State>
  auto getNextStates() const -> std::list<State>;
  virtual auto isEnd() const -> bool = 0;
  virtual auto transitionRandomly() -> void = 0;
  virtual auto getScore() const -> score_t;
};
}
