#pragma once

#include <list>

namespace mcts {
struct AbstractState {
  AbstractState() = default;
  AbstractState(const AbstractState&) = default;
  AbstractState(AbstractState&&) noexcept = default;
  virtual ~AbstractState() = default;
  auto operator=(const AbstractState&) -> AbstractState& = default;
  auto operator=(AbstractState&&) -> AbstractState& = default;
  // Please override the following functions
  template <class State>
  auto getNextStates() const -> std::list<State>;
  auto isEnd() const -> bool;
  auto shiftNextRandomly() -> void;
  auto getScore() const -> int;
};
}
