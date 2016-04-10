#pragma once

#include "node.hpp"

namespace mcts {
class Tree {
 private:
  template <class State>
  using NodesItr = typename Nodes<State>::const_iterator;

  Tree() = delete;
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
  static auto setParams(int playout_number, int playout_limit,
                        double evaluation_param) -> void;
  template <class T, class State = typename std::remove_reference<T>::type>
  static auto search(T&& state) -> State;

  template <class State>
  friend class Node;
};

template <class State>
auto Tree::_selectPlayoutNode(const Nodes<State>& leaf_nodes, int total_count)
  -> NodesItr<State> {
  auto selected_node_pos = leaf_nodes.cend();
  double max_value = -1.0;
  for(auto it = leaf_nodes.cbegin(); it != leaf_nodes.cend(); it++) {
    assert((*it)->isLeafNode());
    auto value = (*it)->getValue(total_count);
    if(value > max_value) {
      selected_node_pos = it;
      max_value = value;
    }
  } 
  assert(selected_node_pos != leaf_nodes.cend());
  return selected_node_pos;
}

template <class State>
inline auto Tree::_isPlayoutLimit(const Node<State>* const node) -> bool {
  return node->count_ >= playout_limit;
}

template <class State>
auto Tree::_update(const Node<State>* const root_node,
                   Nodes<State>& leaf_nodes) -> bool {
  if(leaf_nodes.empty()) return false;
  auto node_pos = Tree::_selectPlayoutNode(leaf_nodes, root_node->count_);
  auto* node = *node_pos;
  node->playout();
  if(Tree::_isPlayoutLimit(node)) {
    node->expand();
    leaf_nodes.erase(node_pos);
    const auto& nodes = node->child_nodes_;
    leaf_nodes.insert(leaf_nodes.cend(), nodes.cbegin(), nodes.cend());
  }
  return true;
}

template <class State>
auto Tree::_selectBetterState(const Node<State>* const root_node) -> State {
  Node<State>* result_node = nullptr;
  double max_value = -1.0;
  const int total_count = root_node->count_;
  assert(!root_node->isLeafNode());
  for(auto* node : root_node->child_nodes_) {
    auto value = node->getValue(total_count);
    if(value > max_value) {
      result_node = node;
      max_value = value;
    }
  }
  assert(result_node != nullptr);
  return result_node->state_;
}

inline auto Tree::setParams(int playout_number, int playout_limit,
                            double evaluation_param) -> void {
  mcts::playout_number = playout_number;
  mcts::playout_limit = playout_limit;
  mcts::evaluation_param = evaluation_param;
}

template <class T, class State>
auto Tree::search(T&& state) -> State {
  if(state.isEnd()) return std::forward<State>(state);
  auto* root_node = new Node<State>(std::forward<State>(state));
  root_node->expand();
  auto leaf_nodes = root_node->child_nodes_;
  for(int i = 0; i < playout_number; i++) {
    if(!Tree::_update(root_node, leaf_nodes)) break;
  }
  auto better_state = Tree::_selectBetterState(root_node); 
  delete root_node;
  return std::move(better_state);
}
}
