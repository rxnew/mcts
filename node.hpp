/**
 * @file node.hpp
 * @brief header of Node class
 */

#pragma once

#include <list>

namespace mcts {
template <class State>
class Node;

template <class State>
using Nodes = std::list<Node<State>*>;

template <class State>
class Node {
 private:
  using score_t = typename State::score_t;

  State state_;
  Node* parent_node_;
  Nodes<State> child_nodes_;
  score_t score_;
  int count_;

  template <class T = State>
  explicit Node(T&& state);
  ~Node();

  auto setChildNodes() -> void;
  auto getExpectedValue() const -> double;
  auto getUcb1Value(int total_count) const -> double;
  auto isLeafNode() const -> bool;
  auto update(score_t score) -> void;
  auto playout() -> void;
  auto expand() -> void;

  friend class Tree;
};
}

#include "impl/node_impl.hpp"
