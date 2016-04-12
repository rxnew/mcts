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
auto Node<State>::setChildNodes() -> bool {
  if(this->state_.isEnd()) return false;
  for(const auto& state : this->state_.template getNextStates<State>()) {
    auto* node = new Node<State>(state);
    node->parent_node_ = this;
    this->child_nodes_.push_back(node);
  }
  return true;
}

template <class State>
inline auto Node<State>::getValue() const -> double {
  assert(this->count_);
  return static_cast<double>(this->score_) / this->count_;
}

template <class State>
auto Node<State>::getValueUct(int total_count) const -> double {
  assert(this->count_);
  auto x = static_cast<double>(this->score_) / this->count_;
  auto y = std::sqrt(std::log(total_count) / this->count_);
  return x + params::exploration_param * y;
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
auto Node<State>::expand() -> bool {
  if(!this->setChildNodes()) return false;
  for(auto* node : this->child_nodes_) {
    node->playout();
  }
  return true;
}
}
