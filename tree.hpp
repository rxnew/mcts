/**
 * @file tree.hpp
 * @brief header of Tree class
 */

#pragma once

#include "node.hpp"

namespace mcts {
class Tree {
 private:
  Tree() = delete;

  template <class State, class F>
  static auto _searchNode(const Nodes<State>& nodes,
                          const F& function) -> Node<State>*;
  template <class State>
  static auto _selectPlayoutNode(Node<State>* const node,
                                 int total_count) -> Node<State>*;
  template <class State>
  static auto _isPlayoutLimit(const Node<State>* const node) -> bool;
  template <class State>
  static auto _update(Node<State>* const root_node) -> void;
  template <class State>
  static auto _selectBetterState(const Node<State>* const root_node) -> State;

 public:
  template <class T, class State = typename std::remove_reference<T>::type>
  static auto search(T&& state) -> State;
};
}

#include "impl/tree_impl.hpp"
