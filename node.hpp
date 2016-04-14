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
  State state_;
  Node* parent_node_;
  Nodes<State> child_nodes_;
  int score_;
  int count_;

  template <class T = State>
  explicit Node(T&& state);
  ~Node();

  auto setChildNodes() -> bool;
  auto getExpectedValue() const -> double;
  auto getUcb1Value(int total_count) const -> double;
  auto isLeafNode() const -> bool;
  auto update(int score) -> void;
  auto playout() -> void;
  auto expand() -> bool;

  friend class Tree;
};
}

#include "impl/node_impl.hpp"
