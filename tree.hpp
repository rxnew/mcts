#pragma once

#include "node.hpp"

namespace mcts {
class Tree {
 private:
  template <class State>
  using NodesItr = typename Nodes<State>::const_iterator;

  Tree() = delete;
  template <class State, class F>
  static auto _getNodePosition(const Nodes<State>& nodes,
                               const F& function) -> NodesItr<State>;
  template <class State>
  static auto _selectPlayoutNode(const Nodes<State>& leaf_nodes,
                                 int total_count) -> NodesItr<State>;
  template <class State>
  static auto _isPlayoutLimit(const Node<State>* const node) -> bool;
  template <class State>
  static auto _update(const Node<State>* const root_node,
                      Nodes<State>& leaf_nodes) -> bool;
  template <class State>
  static auto _selectBetterState(const Node<State>* const root_node) -> State;

 public:
  template <class T, class State = typename std::remove_reference<T>::type>
  static auto search(T&& state) -> State;
};
}

#include "impl/tree_impl.hpp"
