#pragma once

#include <cmath>
#include <cassert>

#include "../params.hpp"

namespace mcts {
template <class State>
template <class T>
inline Node<State>::Node(T&& state)
  : state_(std::forward<T>(state)),
    parent_node_(nullptr), score_(0), count_(0) {
}

template <class State>
Node<State>::~Node() {
  for(auto* node : this->child_nodes_) {
    delete node;
  }
}

template <class State>
auto Node<State>::setChildNodes() -> void {
  if(this->state_.isEnd()) return;
  for(const auto& state : this->state_.template getNextStates<State>()) {
    auto* node = new Node<State>(state);
    node->parent_node_ = this;
    this->child_nodes_.push_back(node);
  }
}

template <class State>
inline auto Node<State>::getExpectedValue() const -> double {
  assert(this->count_);
  return static_cast<double>(this->score_) / this->count_;
}

template <class State>
auto Node<State>::getUcb1Value(int total_count) const -> double {
  assert(this->count_);
  auto bias = std::sqrt(std::log(2 * total_count) / this->count_);
  return this->getExpectedValue() + params::exploration_param * bias;
}

template <class State>
inline auto Node<State>::isLeafNode() const -> bool {
  return this->child_nodes_.empty();
}

template <class State>
auto Node<State>::update(int score) -> void {
  if(this->parent_node_) this->parent_node_->update(score);
  this->score_ += score;
  this->count_++;
}

template <class State>
auto Node<State>::playout() -> void {
  auto state = this->state_;
  while(!state.isEnd()) {
    state.shiftNextRandomly();
  }
  this->update(state.getScore());
}

template <class State>
auto Node<State>::expand() -> void {
  this->setChildNodes();
  for(auto* node : this->child_nodes_) {
    node->playout();
  }
}
}
