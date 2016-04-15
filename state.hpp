/**
 * @file state.hpp
 * @brief header of AbstractState class
 */

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
  auto getNextStates() const -> std::list<typename std::remove_reference<decltype(*this)>::type>;
  virtual auto isEnd() const -> bool = 0;
  virtual auto shiftNextRandomly() -> void = 0;
  virtual auto getScore() const -> int = 0;
};
}
