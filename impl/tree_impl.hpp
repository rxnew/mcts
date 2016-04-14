#pragma once

#include <limits>

namespace mcts {
template <class State, class F>
auto Tree::_searchNode(const Nodes<State>& nodes,
                            const F& function) -> Node<State>* {
  Node<State>* result_node = nullptr;
  auto max_value = std::numeric_limits<double>::lowest();
  for(auto* node : nodes) {
    auto value = function(node);
    if(value > max_value) {
      result_node = node;
      max_value = value;
    }
  }
  assert(result_node != nullptr);
  return result_node;
}

template <class State>
auto Tree::_selectPlayoutNode(Node<State>* const node,
                              int total_count) -> Node<State>* {
  if(node->isLeafNode()) return node;
  auto function = [total_count](const Node<State>* const node) {
    return node->getValueUcb1(total_count);
  };
  auto next_node = Tree::_searchNode(node->child_nodes_, function);
  return Tree::_selectPlayoutNode(next_node, total_count);
}

template <class State>
inline auto Tree::_isPlayoutLimit(const Node<State>* const node) -> bool {
  return node->count_ >= params::playout_limit;
}

template <class State>
auto Tree::_update(Node<State>* const root_node) -> void {
  auto* node = Tree::_selectPlayoutNode(root_node, root_node->count_);
  node->playout();
  if(Tree::_isPlayoutLimit(node)) {
    node->expand();
  }
}

template <class State>
auto Tree::_selectBetterState(const Node<State>* const root_node) -> State {
  assert(!root_node->isLeafNode());
  auto function = [](const Node<State>* const node) {return node->getValue();};
  return (Tree::_searchNode(root_node->child_nodes_, function))->state_;
}

template <class T, class State>
auto Tree::search(T&& state) -> State {
  if(state.isEnd()) return std::forward<State>(state);
  auto* root_node = new Node<State>(std::forward<State>(state));
  root_node->expand();
  for(int i = 0; i < params::playout_number; i++) {
    Tree::_update(root_node);
  }
  auto better_state = Tree::_selectBetterState(root_node); 
  delete root_node;
  return std::move(better_state);
}
}
